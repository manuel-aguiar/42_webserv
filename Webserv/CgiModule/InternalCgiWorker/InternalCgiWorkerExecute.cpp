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
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "../../ServerManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../Globals/Globals.hpp"

// C Headers
# include <unistd.h>
# include <sys/wait.h>

/*

	If any syscall fails, we should let the caller know there was an error, cancel the CgiRequest and reset it
	reset the InternalCgiWorker and return

*/


void   CgiModule::InternalCgiWorker::execute(InternalCgiRequestData& request)
{
	m_curRequestData = &request;

    if (::pipe(m_ParentToChild) == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), pipe(): " + std::string(strerror(errno)));
		m_curRequestData->accessCallback(E_CGI_ON_ERROR_STARTUP).call();
		m_CgiModule.cancelRequest(*m_curRequestData);
		return ;
    }
	if (::pipe(m_ChildToParent) == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), pipe(): " + std::string(strerror(errno)));
		m_curRequestData->accessCallback(E_CGI_ON_ERROR_STARTUP).call();
		m_CgiModule.cancelRequest(*m_curRequestData);
		return ;
	}
	if (!FileDescriptor::setNonBlocking(m_ParentToChild[0]) ||
		!FileDescriptor::setNonBlocking(m_ParentToChild[1]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[0]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[1]))
	{
		m_globals.logError("InternalCgiWorker::execute(), fcntl(): " + std::string(strerror(errno)));
		m_curRequestData->accessCallback(E_CGI_ON_ERROR_STARTUP).call();
		m_CgiModule.cancelRequest(*m_curRequestData);
		return ;
	}

	if (!mf_prepareExecve())
	{
		m_curRequestData->accessCallback(E_CGI_ON_ERROR_STARTUP).call();
		m_CgiModule.cancelRequest(*m_curRequestData);
		return ;
	}

	m_curRequestData->setReadFd(m_ChildToParent[0]);
	m_curRequestData->setWriteFd(m_ParentToChild[1]);
	m_curRequestData->accessCallback(E_CGI_ON_EXECUTE).call();

    m_pid = ::fork();
    if (m_pid == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), fork(): " + std::string(strerror(errno)));
		m_curRequestData->accessCallback(E_CGI_ON_ERROR_RUNTIME).call();
		m_CgiModule.cancelRequest(*m_curRequestData);
		return ;
    }
    if (m_pid == 0)
		mf_executeChild();
	else
		mf_executeParent();
}

bool	CgiModule::InternalCgiWorker::mf_prepareExecve()
{
	
	typedef std::map<t_CgiEnvKey, t_CgiEnvValue>::const_iterator	t_EnvExtraIter;

	const t_CgiRequestEnv& 			envRequest = m_curRequestData->getEnvVars();
	const t_CgiEnvKey*				envBase = m_CgiModule.getBaseEnvKeys();
	size_t							entryCount = envRequest.envExtra.size() + envRequest.envBase.size();
	
	try
	{
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
		return (true);
	}
	catch(const std::exception& e)
	{
		m_globals.logError("InternalCgiWorker::mf_prepareExecve(): " + std::string(e.what()));
	}
	return (false);
}


void	CgiModule::InternalCgiWorker::mf_executeParent()
{
	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ChildToParent[1]);
}

void	CgiModule::InternalCgiWorker::mf_executeChild()
{
	if (::dup2(m_ParentToChild[0], STDIN_FILENO) == -1)
		::exit(EXIT_FAILURE);
	if (::dup2(m_ChildToParent[1], STDOUT_FILENO) == -1)
		::exit(EXIT_FAILURE);
	
	::close(m_ParentToChild[1]);
	::close(m_ChildToParent[0]);

	::execve(m_argPtr[0], m_argPtr.getArray(), m_envPtr.getArray());
	::exit(EXIT_FAILURE);
}