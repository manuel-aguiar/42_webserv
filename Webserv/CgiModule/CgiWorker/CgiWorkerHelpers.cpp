

// Project Headers
# include "CgiWorker.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "../../ServerManager/EventManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../Globals/Globals.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

void	CgiModule::Worker::mf_KillWaitChild()
{
	if (m_pid == -1)
		return ;
	::kill(m_pid, SIGKILL);
	::waitpid(m_pid, NULL, 0);
	m_pid = -1;
}

void	CgiModule::Worker::mf_childFailure()
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
	m_CgiModule.mf_accessGlobals().logError(errorMsg);
	m_CgiModule.mf_recycleRuntimeFailure(*this);
}


void	CgiModule::Worker::mf_waitChild()
{
	int status;
	
	if (m_pid == -1)
		return ;

	::waitpid(m_pid, &status, 0);
	m_pid = -1;

	if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) || WIFSIGNALED(status))
	{
		m_CgiModule.mf_accessGlobals().logError("InternalCgiWorker::mf_executeChild(), child exited with status: " + StringUtils::to_string(status));
		m_CgiModule.mf_recycleRuntimeFailure(*this);
	}
	else
		m_CgiModule.mf_recycleSuccess(*this);
}

void 	CgiModule::Worker::mf_closeFd(t_fd& fd)
{
	if (fd != -1 && ::close(fd) == -1)
		m_CgiModule.mf_accessGlobals().logError("InternalCgiWorker::closeFd(), close(): " + std::string(strerror(errno)));
	fd = -1;
}
