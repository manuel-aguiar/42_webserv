

# include "SignalHandler.hpp"
# include "../Globals.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

SignalHandler 	g_SignalHandler;
size_t			SignalHandler::gm_counter;

SignalHandler::SignalHandler() :
	m_globals(NULL),
	m_pipes(0),
	m_signal(0),
	m_sigact((t_sigaction){})
{
	if (SignalHandler::gm_counter != 0)
		throw std::runtime_error("SignalHandler::SignalHandler: SignalHandler is a singleton");
	SignalHandler::gm_counter++;
}

SignalHandler::~SignalHandler()
{
	for (size_t i = 0; i < m_pipes.size(); ++i)
	{
		if (close(getPipeRead(i)) == -1)
			m_globals->logError("close(): " + std::string(std::strerror(errno)));
		if (close(getPipeWrite(i)) == -1)
			m_globals->logError("close(): " + std::string(std::strerror(errno)));
	}
	m_pipes.clear();	
}

Ws::fd		SignalHandler::getPipeRead(int serverID)
{
	return (m_pipes[serverID].first);
}

Ws::fd		SignalHandler::getPipeWrite(int serverID)
{
	return (m_pipes[serverID].second);
}

int		SignalHandler::getSignal()
{
	return (SignalHandler::m_signal);
};

void	SignalHandler::setSignal(int sig)
{
	m_signal = sig;
}

void		SignalHandler::signal_handler(int sigNum)
{
	size_t count;

	count = g_SignalHandler.getPipes().size();
	if (sigNum == SIGINT || sigNum == SIGQUIT)
	{
		g_SignalHandler.setSignal(sigNum);
		for (size_t i = 0; i < count; ++i)
			write(g_SignalHandler.getPipeWrite(i), "DUKE NUKEM", sizeof("DUKE NUKEM"));
	}
}

const std::vector<std::pair<int, int> >&		SignalHandler::getPipes()
{
	return (m_pipes);
}

void		SignalHandler::prepare_signal(void (*handler)(int), size_t numServers, Globals& globals)
{
	int pipefd[2];

	m_globals = &globals;
	m_pipes.reserve(numServers);

	m_sigact.sa_flags = SA_RESTART;
	m_sigact.sa_handler = handler;

	if (sigemptyset(&(m_sigact.sa_mask)) == -1)
	{
		m_globals->logError("SignalHandler::prepare_signal, sigemptyset(): " + std::string(std::strerror(errno)));
		throw std::runtime_error("SignalHandler::prepare_signal: sigemptyset() failed");
	}

	if (sigaction(SIGINT, &m_sigact, NULL) == -1)
	{
		m_globals->logError("m_sigact(): " + std::string(std::strerror(errno)));
		throw std::runtime_error("SignalHandler::prepare_signal: m_sigact() failed");
	}

	if (sigaction(SIGQUIT, &m_sigact, NULL) == -1)
	{
		m_globals->logError("m_sigact(): " + std::string(std::strerror(errno)));
		throw std::runtime_error("SignalHandler::prepare_signal: m_sigact() failed");
	}

	for (size_t i = 0; i < numServers; ++i)
	{
		if (pipe(pipefd) == -1)
		{
			m_globals->logError("pipe(): " + std::string(std::strerror(errno)));
			throw std::runtime_error("SignalHandler::prepare_signal, pipe() failed");
		}
		if (!FileDescriptor::setCloseOnExec_NonBlocking(pipefd[0])
		|| !FileDescriptor::setCloseOnExec_NonBlocking(pipefd[1]))
		{
			m_globals->logError("fcntl(): " + std::string(std::strerror(errno)));
			throw std::runtime_error("SignalHandler::prepare_signal, fcntl() failed");
		}

		m_pipes.push_back(std::make_pair(pipefd[0], pipefd[1]));
	}
}

// private
SignalHandler::SignalHandler(const SignalHandler& copy)
{
	(void)copy;

	if (SignalHandler::gm_counter != 0)
		throw std::runtime_error("SignalHandler::SignalHandler: SignalHandler is a singleton");
	SignalHandler::gm_counter++;
}

SignalHandler& SignalHandler::operator=(const SignalHandler& assign)
{
	(void)assign;
	
	if (SignalHandler::gm_counter != 0)
		throw std::runtime_error("SignalHandler::SignalHandler: SignalHandler is a singleton");
	SignalHandler::gm_counter++;	
	return (*this);
}