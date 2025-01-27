

// Project Headers
# include "Worker.hpp"
# include "../InternalReq/InternalReq.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

# include "../../Globals/Globals.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

/*
	Prepare pipes, prepare fds, prepare env vars and execve arguments.

	Inform the user that execution will begin, user can prepare its setup based on the pipes provided
	(subscribind read and write ends to its epoll, for example)

	Fork, child does child, parent does parent
*/

void   Worker::execute(bool markFdsAsStale)
{
	Cgi::Options::Mask options;

	options = m_curRequestData->getOptions();

	if (!mf_prepareExecve())
		return (mf_recycleStartupFailure(markFdsAsStale));

	if (::pipe(m_ParentToChild) == -1 ||
		::pipe(m_ChildToParent) == -1 ||
		::pipe(m_EmergencyPhone) == -1)
	{
		mf_accessGlobals().logError("InternalCgiWorker::execute(), pipe(): " + std::string(strerror(errno)));
		return (mf_recycleStartupFailure(markFdsAsStale));
	}

	//std::cout << "\nopening fd: " << (m_EmergencyPhone[0]) << ", phone read " << '\n';
	//std::cout << "opening fd: " << (m_EmergencyPhone[1]) << ", phone write " << '\n';
	//std::cout << "opening fd: " << (m_ParentToChild[0]) << ", child read " << '\n';
	//std::cout << "opening fd: " << (m_ParentToChild[1]) << ", parent write " <<  '\n';
	//std::cout << "opening fd: " << (m_ChildToParent[0]) << ", parent read " <<  '\n';
	//std::cout << "opening fd: " << (m_ChildToParent[1]) << ", child write " <<  '\n';

	if (!(options & Cgi::Options::CANCEL_WRITE))
	{
		m_writeEvent->setFd(m_ParentToChild[1]);
		mf_accessEventManager().startMonitoring(*m_writeEvent, markFdsAsStale);
	}

	if (!(options & Cgi::Options::CANCEL_READ))
	{
		m_readEvent->setFd(m_ChildToParent[0]);
		mf_accessEventManager().startMonitoring(*m_readEvent, markFdsAsStale);
	}

	m_EmergencyEvent->setFd(m_EmergencyPhone[0]);
	mf_accessEventManager().startMonitoring(*m_EmergencyEvent, markFdsAsStale);
	

	//std::cout << "\nopening fd: " << (m_EmergencyPhone[0]) << ", phone read " << '\n';
	//std::cout << "opening fd: " << (m_EmergencyPhone[1]) << ", phone write " << '\n';
	//std::cout << "opening fd: " << (m_ParentToChild[0]) << ", child read " << '\n';
	//std::cout << "opening fd: " << (m_ParentToChild[1]) << ", parent write " <<  '\n';
	//std::cout << "opening fd: " << (m_ChildToParent[0]) << ", parent read " <<  '\n';
	//std::cout << "opening fd: " << (m_ChildToParent[1]) << ", child write " <<  '\n';

	m_pid = ::fork();
	if (m_pid == -1)
	{
		mf_accessGlobals().logError("InternalCgiWorker::execute(), fork(): " + std::string(strerror(errno)));
		return (mf_recycleStartupFailure(markFdsAsStale));
	}
	

	if (m_pid == 0)
		mf_executeChild();
	else
		mf_executeParent(markFdsAsStale);
	
}


/*
	Parent just closes the pipe ends that doesn't use and simply waits for the Manager
	to inform it that the child process has exited -> Events
*/
void	Worker::mf_executeParent(bool markFdsAsStale)
{
	Cgi::Options::Mask options;

	options = m_curRequestData->getOptions();

	if ((!(options & Cgi::Options::CANCEL_WRITE) && !FileDescriptor::setNonBlocking(m_ParentToChild[1])) ||
		(!(options & Cgi::Options::CANCEL_READ) && !FileDescriptor::setNonBlocking(m_ChildToParent[0]))  ||
		!FileDescriptor::setNonBlocking(m_EmergencyPhone[0]))
	{
		mf_accessGlobals().logError("InternalCgiWorker::execute(), fcntl(): " + std::string(strerror(errno)));
		return (mf_recycleStartupFailure(markFdsAsStale));
	}

	if (options & Cgi::Options::CANCEL_WRITE)
		mf_closeFd(m_ParentToChild[1]);
	if (options & Cgi::Options::CANCEL_READ)
		mf_closeFd(m_ChildToParent[0]);
	if (options & Cgi::Options::HOLD_READ)
		disableReadHandler();
	if (options & Cgi::Options::HOLD_WRITE)
		disableWriteHandler();

	mf_closeFd(m_EmergencyPhone[1]);
	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ChildToParent[1]);

	m_EmergencyPhone[0] = -1;
	m_ParentToChild[1] = -1;
	m_ChildToParent[0] = -1;
}

/*
	The EmergencyCode message that the Child process may send to the parent is made of 2 bytes.

	The first byte is just the enum value of the syscall that failed. In our case, there are
	two major possibilities: dup2 (failed to redirect), execve(failed to execute the script)

	The second byte is the errno value recorded when the syscall failed, such that the parent worker
	can log the info correctly and then tell the User what happened.

	The parent will subscribe the emergencyphone event in the epoll, so epoll will inform it when the
	child process exits, whether with a message or a straight EOF.

	If everything goes according to plan, the process by which child is replaced in execve will close
	the pipe as it exits, the parent will receive an EOF directly and no other info, letting the parent
	know that everything went smoothly, and to peacefully waitpid the child.
*/

static void childCloseFd(Ws::fd fd)
{
	if (fd != -1)
		::close(fd);
	//std::cerr << " child closed " << fd << std::endl;
}




void	Worker::mf_executeChild()
{
	char EmergencyCode[2];

	childCloseFd(m_EmergencyPhone[0]);
	childCloseFd(m_ParentToChild[1]);
	childCloseFd(m_ChildToParent[0]);

	if (::dup2(m_ParentToChild[0], STDIN_FILENO) == -1 ||
		::dup2(m_ChildToParent[1], STDOUT_FILENO) == -1)
	{
		EmergencyCode[0] = E_EMER_DUP2;
		EmergencyCode[1] = errno;
		::write(m_EmergencyPhone[1], EmergencyCode, 2);

		childCloseFd(m_EmergencyPhone[1]);
		childCloseFd(m_ParentToChild[0]);
		childCloseFd(m_ChildToParent[1]);
				
		
		// wait to be killed lol
		while (1)
			::usleep(1000);
	}
		
	::execve(m_argPtr[0], m_argPtr.getArray(), m_envPtr.getArray());

	EmergencyCode[0] = E_EMER_EXECVE;
	EmergencyCode[1] = errno;
	::write(m_EmergencyPhone[1], EmergencyCode, 2);
	
	childCloseFd(m_EmergencyPhone[1]);
	childCloseFd(m_ParentToChild[0]);
	childCloseFd(m_ChildToParent[1]);
	
	//wait to be killed lol
	while (1)
		::usleep(1000);
}


/*
	Putting env variables in place for execution.
	Try-catch because the DynArray may throw if it fails to allocate memory
*/

