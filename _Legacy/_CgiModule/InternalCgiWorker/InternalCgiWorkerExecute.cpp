/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorkerExecute.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 09:25:41 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 17:05:07 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
# include "InternalCgiWorker.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../CgiRequestData/CgiRequestData.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../Globals/Globals.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

void   CgiModule::InternalCgiWorker::execute(CgiRequestData& request)
{
	m_curRequestData = &request;
	m_curEventManager = &m_curRequestData->accessEventManager();
	

	// more asserts, all data must be in place and ready to execute
	assert(m_curEventManager != NULL && m_curRequestData != NULL);

    if (::pipe(m_ParentToChild) == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), pipe(): " + std::string(strerror(errno)));
		return (m_curRequestData->accessCallbacks(E_CGI_ON_ERROR).handle());
    }
	if (::pipe(m_ChildToParent) == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), pipe(): " + std::string(strerror(errno)));
		return (m_curRequestData->accessCallbacks(E_CGI_ON_ERROR).handle());
	}
	if (!FileDescriptor::setNonBlocking(m_ParentToChild[0]) ||
		!FileDescriptor::setNonBlocking(m_ParentToChild[1]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[0]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[1]))
	{
		m_globals.logError("InternalCgiWorker::execute(), fcntl(): " + std::string(strerror(errno)));
		return (m_curRequestData->accessCallbacks(E_CGI_ON_ERROR).handle());
	}

	mf_prepareExecve();

    m_pid = ::fork();
    if (m_pid == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), fork(): " + std::string(strerror(errno)));
		return (m_curRequestData->accessCallbacks(E_CGI_ON_ERROR).handle());
    }
    if (m_pid == 0)
		mf_executeChild();
	else
		mf_executeParent();
}

void	CgiModule::InternalCgiWorker::mf_prepareExecve()
{
	
	typedef std::map<t_CgiEnvKey, t_CgiEnvValue>::const_iterator	t_EnvExtraIter;

	const t_CgiRequestEnv& 			envRequest = m_curRequestData->getEnvVars();
	const t_CgiEnvKey*				envBase = m_CgiModule.getBaseEnvKeys();
	size_t							entryCount = envRequest.envExtra.size() + envRequest.envBase.size();
	
	m_envStr.reserve(entryCount);
	m_envPtr.reserve(entryCount + 1);
	m_argPtr.reserve(3);

	for (size_t i = 0; i < envRequest.envBase.size(); i++)
		m_envStr.push_back(envBase[envRequest.envBase[i].first] + "=" + envRequest.envBase[i].second);

	for (t_EnvExtraIter it = envRequest.envExtra.begin(); it != envRequest.envExtra.end(); it++)
		m_envStr.push_back(it->first + "=" + it->second);

	for (size_t i = 0; i < entryCount; i++)
		m_envPtr.push_back(const_cast<char*>(m_envStr[i].c_str()));
	m_envPtr.push_back(NULL);

	m_argPtr.push_back(const_cast<char*>(m_CgiModule.getInterpreters().find(m_curRequestData->getExtension())->second.c_str()));
	m_argPtr.push_back(const_cast<char*>(m_curRequestData->getScriptPath().c_str()));
	m_argPtr.push_back(NULL);
}


void	CgiModule::InternalCgiWorker::mf_executeParent()
{
	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ChildToParent[1]);
	
	m_writeEvent.setFd(m_ParentToChild[1]);
	m_readEvent.setFd(m_ChildToParent[0]);

	m_curEventManager->addEvent(m_writeEvent);
	m_curEventManager->addEvent(m_readEvent);

}

void	CgiModule::InternalCgiWorker::mf_executeChild()
{
	//std::cerr << "executing child" << std::endl;

	
	if (::dup2(m_ParentToChild[0], STDIN_FILENO) == -1)
	//{
		//std::cerr << "first dup2 failed" << std::endl;
		::exit(EXIT_FAILURE);
	//}
	//else
	//	std::cerr << "first dup2 success" << std::endl;

	if (::dup2(m_ChildToParent[1], STDOUT_FILENO) == -1)
	//{
	//	std::cerr << "second dup2 failed" << std::endl;
		::exit(EXIT_FAILURE);
	//}
	//else
	//	std::cerr << "second dup2 success" << std::endl;
		


	//std::cerr << "calling execve" << std::endl;
	::execve(m_argPtr[0], m_argPtr.getArray(), m_envPtr.getArray());
	//std::cerr << "execve failed" << std::endl;
	::exit(EXIT_FAILURE);
}