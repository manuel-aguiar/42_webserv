

// Project Headers
# include "Worker.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

// C++ headers
# include <cerrno>

// C Headers
# include <unistd.h>
# include <sys/wait.h>

void	Worker::mf_KillWaitChild()
{
	if (m_pid == -1)
		return ;
	::kill(m_pid, SIGKILL);
	::waitpid(m_pid, NULL, 0);
	m_pid = -1;
}

void	Worker::mf_childFailure()
{
	std::string errorMsg;
	
	switch (m_EmergencyBuffer[0])
	{
		case E_EMER_DUP2:
			errorMsg = "InternalCgiWorker::mf_executeChild(), dup2(): "; break ;
		case E_EMER_EXECVE:
			errorMsg = "InternalCgiWorker::mf_executeChild(), execve(): "; break ;
		default : break;
	}
	if (m_EmergencyBytesRead == 2)
		errorMsg += ::strerror(m_EmergencyBuffer[1]);
	else
		errorMsg += "inconclusive error";
	mf_accessGlobals().logError(errorMsg);
	mf_recycleRuntimeFailure();
}


void	Worker::mf_waitChild()
{
	int status;
	
	if (m_pid == -1)
		return ;

	::waitpid(m_pid, &status, 0);
	m_pid = -1;

	if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) || WIFSIGNALED(status))
	{
		
		mf_accessGlobals().logError("InternalCgiWorker::mf_executeChild(), child exited with status: " + StringUtils::to_string(status));
		mf_recycleRuntimeFailure();
	}
	else
	{
		std::cerr << "recycle success" << std::endl;
		mf_recycleSuccess();
	}
}

void 	Worker::mf_closeFd(Ws::fd& fd)
{
	if (fd != -1 && ::close(fd) == -1)
		mf_accessGlobals().logError("InternalCgiWorker::closeFd(), close(): " + std::string(strerror(errno)));
	fd = -1;
}
