/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequestExecute.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:25:41 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/17 12:17:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
# include "CgiLiveRequest.hpp"
# include "../CgiModule.hpp"
# include "../CgiRequestData/CgiRequestData.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../Globals/Globals.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

void   CgiLiveRequest::execute()
{
	assert(m_curEventManager != NULL && m_curRequestData != NULL);

    if (::pipe(m_ParentToChild) == -1)
	{
		m_globals.logError("CgiLiveRequest::execute(), pipe(): " + std::string(strerror(errno)));
		return (m_curRequestData->getEventHandler(CGI_ON_ERROR).handle());
    }
	if (::pipe(m_ChildToParent) == -1)
	{
		m_globals.logError("CgiLiveRequest::execute(), pipe(): " + std::string(strerror(errno)));
		return (m_curRequestData->getEventHandler(CGI_ON_ERROR).handle());
	}
	if (!FileDescriptor::setNonBlocking(m_ParentToChild[0]) ||
		!FileDescriptor::setNonBlocking(m_ParentToChild[1]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[0]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[1]))
	{
		m_globals.logError("CgiLiveRequest::execute(), fcntl(): " + std::string(strerror(errno)));
		return (m_curRequestData->getEventHandler(CGI_ON_ERROR).handle());
	}

	mf_prepareExecve();

    m_pid = ::fork();
    if (m_pid == -1)
	{
		m_globals.logError("CgiLiveRequest::execute(), fork(): " + std::string(strerror(errno)));
		return (m_curRequestData->getEventHandler(CGI_ON_ERROR).handle());
    }
    if (m_pid == 0)
		mf_executeChild();
	else
		mf_executeParent();
}

void	CgiLiveRequest::mf_prepareExecve()
{
	size_t					entries;
	
	entries = m_curRequestData->getEnvBase().size() + m_curRequestData->getEnvExtra().size();
	m_envStr.reserve(entries);
	m_envPtr.reserve(entries + 1);
	m_argPtr.reserve(3);

	for (std::map<e_CgiEnv, t_CgiEnvValue>::const_iterator it = m_curRequestData->getEnvBase().begin(); it != m_curRequestData->getEnvBase().end(); it++)
		m_envStr.push_back(m_CgiModule.getBaseEnvKeys().find(it->first)->second + "=" + it->second);

	for (std::map<t_CgiEnvKey, t_CgiEnvValue>::const_iterator it = m_curRequestData->getEnvExtra().begin(); it != m_curRequestData->getEnvExtra().end(); it++)
		m_envStr.push_back(it->first + "=" + it->second);

	for (size_t i = 0; i < entries; i++)
		m_envPtr.push_back(const_cast<char*>(m_envStr[i].c_str()));
	m_envPtr.push_back(NULL);

	m_argPtr.push_back(const_cast<char*>(m_CgiModule.getInterpreters().find(m_curRequestData->getExtension())->second.c_str()));
	m_argPtr.push_back(const_cast<char*>(m_curRequestData->getScriptPath().c_str()));
	m_argPtr.push_back(NULL);
}


void	CgiLiveRequest::mf_executeParent()
{
	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ChildToParent[1]);
	
	m_writeEvent.setFd(m_ParentToChild[1]);
	m_readEvent.setFd(m_ChildToParent[0]);

	m_curEventManager->addEvent(m_writeEvent);
	m_curEventManager->addEvent(m_readEvent);


/*	
	const char* postData = "sample data for POST request";
	::write(m_ParentToChild[1], postData, strlen(postData));
	::close(m_ParentToChild[1]);
*/
	/**************************/
	// subscribe pipe read to epoll
	/***************************/

/*
	int status;
	::waitpid(m_pid, &status, 0);
*/


}

void	CgiLiveRequest::mf_executeChild()
{


	mf_closeFd(m_ParentToChild[1]);
	mf_closeFd(m_ChildToParent[0]);
	
	if (::dup2(m_ParentToChild[0], STDIN_FILENO) == -1)
		::exit(EXIT_FAILURE);

	if (::dup2(m_ChildToParent[1], STDOUT_FILENO) == -1)
		::exit(EXIT_FAILURE);
	::close(m_ParentToChild[1]);


	::execve(m_argPtr[0], m_argPtr.getArray(), m_envPtr.getArray());
	::exit(EXIT_FAILURE);
}