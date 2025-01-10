/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorkerHelpers.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:41:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 15:27:24 by mmaria-d         ###   ########.fr       */
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

	if (m_pid != -1)
	{
		::waitpid(m_pid, &status, 0);
		m_pid = -1;
		if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) || WIFSIGNALED(status))
			mf_CallTheUser(E_CGI_ON_ERROR_RUNTIME);
	}
}

void	CgiModule::InternalCgiWorker::mf_KillWaitChild()
{
	int status;

	if (m_pid != -1)
	{
		//std::cout << "\t\t\t\t\t KILLING A PROCESS" << std::endl;
		::kill(m_pid, SIGKILL);
		::waitpid(m_pid, &status, 0);
		m_pid = -1;
	}
}

void	CgiModule::InternalCgiWorker::mf_CallTheUser(const e_CgiCallback event)
{
	Callback& callback = m_curRequestData->accessCallback(event);

	if (callback.getHandler() == NULL)
		return ;
	callback.execute();
}


void 	CgiModule::InternalCgiWorker::mf_closeFd(t_fd& fd)
{
	if (fd != -1 && ::close(fd) == -1)
		m_globals.logError("InternalCgiWorker::closeFd(), close(): " + std::string(strerror(errno)));
	fd = -1;
}
