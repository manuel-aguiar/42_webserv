/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorkerHelpers.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:41:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/17 17:33:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
# include "InternalCgiWorker.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "../../ServerManager/EventManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../Globals/Globals.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

void	CgiModule::InternalCgiWorker::mf_KillWaitChild()
{
	if (m_pid == -1)
		return ;
	::kill(m_pid, SIGKILL);
	::waitpid(m_pid, NULL, 0);
	m_pid = -1;
}

void	CgiModule::InternalCgiWorker::mf_childFailure()
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
	m_globals.logError(errorMsg);
	m_CgiModule.mf_recycleRuntimeFailure(*this);
}


void	CgiModule::InternalCgiWorker::mf_waitChild()
{
	int status;
	
	if (m_pid == -1)
		return ;

	::waitpid(m_pid, &status, 0);
	m_pid = -1;

	if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) || WIFSIGNALED(status))
	{
		m_globals.logError("InternalCgiWorker::mf_executeChild(), child exited with status: " + StringUtils::to_string(status));
		m_CgiModule.mf_recycleRuntimeFailure(*this);
	}
	else
		m_CgiModule.mf_recycleSuccess(*this);
}

void 	CgiModule::InternalCgiWorker::mf_closeFd(t_fd& fd)
{
	if (fd != -1 && ::close(fd) == -1)
		m_globals.logError("InternalCgiWorker::closeFd(), close(): " + std::string(strerror(errno)));
	if (fd != -1)
		//std::cout << "parent closing fd " << fd << std::endl;
	fd = -1;
}
