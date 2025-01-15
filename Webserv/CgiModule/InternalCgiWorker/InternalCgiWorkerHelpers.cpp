/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorkerHelpers.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:41:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/15 14:28:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
# include "InternalCgiWorker.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../Globals/Globals.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>



void	CgiModule::InternalCgiWorker::mf_JustWaitChild()
{
	int status;

	if (m_pid == -1)
		return ;

	::waitpid(m_pid, &status, 0);
	m_pid = -1;

	if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) || WIFSIGNALED(status))
		m_curRequestData->CallTheUser(E_CGI_ON_ERROR_RUNTIME);
}

void	CgiModule::InternalCgiWorker::mf_KillWaitChild()
{
	int status;

	if (m_pid == -1)
		return ;
		
	::kill(m_pid, SIGKILL);
	::waitpid(m_pid, &status, 0);
	m_pid = -1;
	
	//m_CgiModule.mf_stopExecutionPrepareCleanup(*m_curRequestData);
}



void 	CgiModule::InternalCgiWorker::mf_closeFd(t_fd& fd)
{
	if (fd != -1 && ::close(fd) == -1)
		m_globals.logError("InternalCgiWorker::closeFd(), close(): " + std::string(strerror(errno)));
	fd = -1;
}
