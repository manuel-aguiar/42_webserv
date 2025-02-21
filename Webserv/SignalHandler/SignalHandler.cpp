

# include "SignalHandler.hpp"
# include "../Globals/Globals.hpp"
# include "../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstring>
# include <cerrno>
# include <stdexcept>

// C headers
# include <unistd.h>
# include <signal.h>

//the user of the class must declare this global
//user may want to derive from this class, let it declare the global
SignalHandler 	g_SignalHandler;

bool 			SignalHandler::m_isInstantiated = false;

/*
	Signal handler that will be subscribed by our class to all signals that get registered
*/
void
SignalHandler::mf_defaultHandler(const int sigNum)
{
	g_SignalHandler.notifySignalReception(sigNum);
}

/*
	There can only be one signal handler in the program.ADJ_NANO
	It must be global such that it can be called from the signal handler
	However, we use asserts to require uniqueness of the instance
	User must declare the global itself (if the user wants to derive from this class
	they can declare their own global without being tied to ours),
	hence extern above
*/
SignalHandler::SignalHandler() :
	m_signal(SIG_NONE),
	m_modifiedSignals(0)
{
	ASSERT_EQUAL(m_isInstantiated == 0, true, "SignalHandler can only be instantiated once");
	SignalHandler::m_isInstantiated = true;
}

/*
	Just close the pipes
*/
SignalHandler::~SignalHandler()
{
	mf_clearOpenPipes();
}

/*
	Function to be called by the server workers to get their own pipe read.
	Each worker will subscribe this listener to their epoll to monitor signals.ADJ_NANO
	This way, signal reception is solved synchronously by the worker
*/
Ws::fd
SignalHandler::getSignalListener(const int serverID)
{
	return (m_pipes[serverID].read);
}

int	
SignalHandler::getSignal()
{
	return (SignalHandler::m_signal);
};

void
SignalHandler::setSignal(int sig)
{
	m_signal = sig;
}



/*
	This function will be called by the mf_defaultHandler upon reception of a signal
	It will write to all the pipes which signal it was such that the receiver can
	identify and act accordingly (our use case, the listener will just exit)
*/
void
SignalHandler::notifySignalReception(const int sigNum)
{
	setSignal(sigNum);
	size_t count = m_pipes.size();

	for (size_t i = 0; i < count; ++i)
	{
		int bytesWritten = write(m_pipes[i].write, &sigNum, 1);
		(void)bytesWritten;
	}
}

void
SignalHandler::openSignalListeners(const size_t numServers, Globals* globals)
{
	Ws::fd pipefd[2];

	mf_clearOpenPipes();
	m_pipes.reserve(numServers);

	for (size_t i = 0; i < numServers; ++i)
	{
		if (pipe(pipefd) == -1)
		{
			if (globals)
				globals->logError("pipe(): " + std::string(std::strerror(errno)));
			throw std::runtime_error("SignalHandler::prepare_signal, pipe() failed");
		}
		if (!FileDescriptor::setCloseOnExec_NonBlocking(pipefd[0])
		|| !FileDescriptor::setCloseOnExec_NonBlocking(pipefd[1]))
		{
			if (globals)
				globals->logError("fcntl(): " + std::string(std::strerror(errno)));
			throw std::runtime_error("SignalHandler::prepare_signal, fcntl() failed");
		}

		m_pipes.emplace_back((PipeFds){.read = pipefd[0], .write = pipefd[1]});
	}
}

void
SignalHandler::registerSignal(const int sigNum, Globals* globals)
{
	struct sigaction sigact;

	ASSERT_EQUAL(sigNum <= 64, true, "SignalHandler::registerSignal: Signal number out of range");

	sigact.sa_flags = SA_RESTART;
	sigact.sa_handler = mf_defaultHandler;
	::sigemptyset(&(sigact.sa_mask));

	if (::sigaction(sigNum, &sigact, NULL) == -1)
	{
		if (globals)
			globals->logError("m_sigact(): " + std::string(std::strerror(errno)));
		throw std::runtime_error("SignalHandler::registerSignal: m_sigact() failed");
	}

	m_modifiedSignals |= (1 << sigNum);
}

void
SignalHandler::unregisterSignal(const int sigNum, Globals* globals)
{
	struct sigaction sigact;	

	ASSERT_EQUAL(sigNum <= 64, true, "SignalHandler::unregisterSignal: Signal number out of range");
	ASSERT_EQUAL((m_modifiedSignals & (1 << sigNum)) != 0, true, "SignalHandler::unregisterSignal: Signal not registered in the first place");

	sigact.sa_flags = 0;
	sigact.sa_handler = SIG_DFL;
	::sigemptyset(&(sigact.sa_mask));

	if (::sigaction(sigNum, &sigact, NULL) == -1)
	{
		if (globals)
			globals->logError("m_sigact(): " + std::string(std::strerror(errno)));
		throw std::runtime_error("SignalHandler::unregisterSignal: m_sigact() failed");
	}

	m_modifiedSignals &= ~(1 << sigNum);
}

void
SignalHandler::ignoreSignal(const int sigNum, Globals* globals)
{
	struct sigaction sigact;	

	ASSERT_EQUAL(sigNum <= 64, true, "SignalHandler::ignoreSignal: Signal number out of range");

	sigact.sa_flags = 0;
	sigact.sa_handler = SIG_IGN;
	::sigemptyset(&(sigact.sa_mask));

	if (::sigaction(sigNum, &sigact, NULL) == -1)
	{
		if (globals)
			globals->logError("m_sigact(): " + std::string(std::strerror(errno)));
		throw std::runtime_error("SignalHandler::ignoreSignal: m_sigact() failed");
	}

	m_modifiedSignals |= (1 << sigNum);
}

bool
SignalHandler::isSignalRegistered(const int sigNum)
{
	return(m_modifiedSignals & (1 << sigNum));
}

/*
	Binary search to find the first set bit in a number
*/
static int findFirstSetBitPosition(size_t value)
{
	int pos = 0;

	if (!value)
		return (-1);
	if ((value & 0xffffffff) == 0) {
		pos += 32;
		value >>= 32;
	}
	if ((value & 0xffff) == 0) {
		pos += 16;
		value >>= 16;
	}
	if ((value & 0xff) == 0) {
		pos += 8;
		value >>= 8;
	}
	if ((value & 0xf) == 0) {
		pos += 4;
		value >>= 4;
	}
	if ((value & 0x3) == 0) {
		pos += 2;
		value >>= 2;
	}
	if ((value & 0x1) == 0) {
		pos += 1;
	}

    return (pos);
}

void
SignalHandler::reset(Globals* globals)
{
	m_signal = SIG_NONE;
	mf_clearOpenPipes();

    size_t modifiedSignals = m_modifiedSignals;

    int i = -1;
    while (modifiedSignals)
    {
		int pos = findFirstSetBitPosition(modifiedSignals) + 1;

		i += pos;
        unregisterSignal(i, globals);

        modifiedSignals >>= pos;
    }
	m_modifiedSignals = 0;
}

void
SignalHandler::mf_clearOpenPipes()
{
	for (size_t i = 0; i < m_pipes.size(); ++i)
	{
		::close(m_pipes[i].read);
		::close(m_pipes[i].write);
	}
	m_pipes.clear();
}

// private
SignalHandler::SignalHandler(const SignalHandler& copy)
{
	(void)copy;

	ASSERT_EQUAL(m_isInstantiated == false, true, "SignalHandler can't be copied");
	SignalHandler::m_isInstantiated = true;
}

SignalHandler&
SignalHandler::operator=(const SignalHandler& assign)
{
	(void)assign;
	
	ASSERT_EQUAL(m_isInstantiated == false, true, "SignalHandler can't be copied");
	SignalHandler::m_isInstantiated = true;	
	return (*this);
}