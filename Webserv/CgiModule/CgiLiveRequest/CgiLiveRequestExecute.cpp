/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiLiveRequestExecute.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:25:41 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 10:02:27 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
# include "CgiLiveRequest.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../CgiRequestData/CgiRequestData.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../Globals/Globals.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

void   CgiModule::CgiLiveRequest::execute(CgiRequestData& request)
{
	m_curEventManager = &request.accessEventManager();
	m_curRequestData = &request;
	assert(m_curEventManager != NULL && m_curRequestData != NULL);

    if (::pipe(m_ParentToChild) == -1)
	{
		m_globals.logError("CgiLiveRequest::execute(), pipe(): " + std::string(strerror(errno)));
		return (m_curRequestData->accessEventHandler(E_CGI_ON_ERROR).handle());
    }
	if (::pipe(m_ChildToParent) == -1)
	{
		m_globals.logError("CgiLiveRequest::execute(), pipe(): " + std::string(strerror(errno)));
		return (m_curRequestData->accessEventHandler(E_CGI_ON_ERROR).handle());
	}
	if (!FileDescriptor::setNonBlocking(m_ParentToChild[0]) ||
		!FileDescriptor::setNonBlocking(m_ParentToChild[1]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[0]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[1]))
	{
		m_globals.logError("CgiLiveRequest::execute(), fcntl(): " + std::string(strerror(errno)));
		return (m_curRequestData->accessEventHandler(E_CGI_ON_ERROR).handle());
	}

	mf_prepareExecve();

    m_pid = ::fork();
    if (m_pid == -1)
	{
		m_globals.logError("CgiLiveRequest::execute(), fork(): " + std::string(strerror(errno)));
		return (m_curRequestData->accessEventHandler(E_CGI_ON_ERROR).handle());
    }
    if (m_pid == 0)
		mf_executeChild();
	else
		mf_executeParent();
}

void	CgiModule::CgiLiveRequest::mf_prepareExecve()
{
	typedef std::map<e_CgiEnv, t_CgiEnvValue>::const_iterator		t_EnvBaseIter;
	typedef std::map<t_CgiEnvKey, t_CgiEnvValue>::const_iterator	t_EnvExtraIter;

	const std::map<e_CgiEnv, t_CgiEnvValue>& 		EnvBase = m_curRequestData->getEnvBase();
	const std::map<t_CgiEnvKey, t_CgiEnvValue>& 	EnvExtra = m_curRequestData->getEnvExtra();
	size_t											entryCount = EnvBase.size() + EnvExtra.size();
	
	m_envStr.reserve(entryCount);
	m_envPtr.reserve(entryCount + 1);
	m_argPtr.reserve(3);

	for (t_EnvBaseIter it = EnvBase.begin(); it != EnvBase.end(); it++)
		m_envStr.push_back(m_CgiModule.getBaseEnvKeys().find(it->first)->second + "=" + it->second);

	for (t_EnvExtraIter it = EnvExtra.begin(); it != EnvExtra.end(); it++)
		m_envStr.push_back(it->first + "=" + it->second);

	for (size_t i = 0; i < entryCount; i++)
		m_envPtr.push_back(const_cast<char*>(m_envStr[i].c_str()));
	m_envPtr.push_back(NULL);

	m_argPtr.push_back(const_cast<char*>(m_CgiModule.getInterpreters().find(m_curRequestData->getExtension())->second.c_str()));
	m_argPtr.push_back(const_cast<char*>(m_curRequestData->getScriptPath().c_str()));
	m_argPtr.push_back(NULL);
}


void	CgiModule::CgiLiveRequest::mf_executeParent()
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

void	CgiModule::CgiLiveRequest::mf_executeChild()
{
	std::cout << "executing child" << std::endl;

	
	if (::dup2(m_ParentToChild[0], STDIN_FILENO) == -1)
	{
		std::cout << "first dup2 failed" << std::endl;
		::exit(EXIT_FAILURE);
	}
	else
		std::cout << "first dup2 success" << std::endl;

	if (::dup2(m_ChildToParent[1], STDOUT_FILENO) == -1)
	{
		std::cout << "second dup2 failed" << std::endl;
		::exit(EXIT_FAILURE);
	}
	else
		std::cout << "second dup2 success" << std::endl;
		


	std::cout << "calling execve" << std::endl;
	::execve(m_argPtr[0], m_argPtr.getArray(), m_envPtr.getArray());
	std::cout << "execve failed" << std::endl;
	::exit(EXIT_FAILURE);
}