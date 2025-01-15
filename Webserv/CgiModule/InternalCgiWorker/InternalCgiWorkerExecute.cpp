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
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
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
void   CgiModule::InternalCgiWorker::execute(InternalCgiRequestData& request)
{
	m_curRequestData = &request;

    if (::pipe(m_ParentToChild) == -1 ||
		::pipe(m_ChildToParent) == -1 ||
		::pipe(m_EmergencyPhone) == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), pipe(): " + std::string(strerror(errno)));
		m_CgiModule.mf_returnRequestData(*m_curRequestData);
		m_CgiModule.mf_returnWorker(*this);
		return (m_curRequestData->CallTheUser(E_CGI_ON_ERROR_STARTUP));
    }
	if (!FileDescriptor::setNonBlocking(m_ParentToChild[0]) ||
		!FileDescriptor::setNonBlocking(m_ParentToChild[1]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[0]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[1])||
		!FileDescriptor::setNonBlocking(m_EmergencyPhone[0]) ||
		!FileDescriptor::setNonBlocking(m_EmergencyPhone[1]))
	{
		m_globals.logError("InternalCgiWorker::execute(), fcntl(): " + std::string(strerror(errno)));
		m_CgiModule.mf_returnRequestData(*m_curRequestData);
		m_CgiModule.mf_returnWorker(*this);
		return (m_curRequestData->CallTheUser(E_CGI_ON_ERROR_STARTUP));
	}

	if (!mf_prepareExecve())
	{
		m_CgiModule.mf_returnRequestData(*m_curRequestData);
		m_CgiModule.mf_returnWorker(*this);
		return (m_curRequestData->CallTheUser(E_CGI_ON_ERROR_STARTUP));
	}

	m_curRequestData->setReadFd(m_ChildToParent[0]);
	m_curRequestData->setWriteFd(m_ParentToChild[1]);
	m_EmergencyEvent.setFd(m_EmergencyPhone[0]);

	m_curRequestData->CallTheUser(E_CGI_ON_EXECUTE);
	m_curRequestData->accessEventManager()->addEvent(m_EmergencyEvent);

    m_pid = ::fork();
    if (m_pid == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), fork(): " + std::string(strerror(errno)));
		m_CgiModule.mf_returnRequestData(*m_curRequestData);
		m_CgiModule.mf_returnWorker(*this);
		return (m_curRequestData->CallTheUser(E_CGI_ON_ERROR_RUNTIME));
    }
	

    if (m_pid == 0)
		mf_executeChild();
	else
		mf_executeParent();
	
}

/*
	Putting env variables in place for execution.
	Try-catch because the DynArray may throw if it fails to allocate memory
*/
bool	CgiModule::InternalCgiWorker::mf_prepareExecve()
{
	typedef DynArray<std::pair<t_CgiEnvKey, t_CgiEnvValue> >::const_iterator	t_EnvExtraIter;

	assert(m_curRequestData->accessEventManager() != NULL);

	const t_CgiRequestEnv& 			envRequest = m_curRequestData->getEnvVars();
	const t_CgiEnvValue*				envBase = m_CgiModule.getBaseEnvKeys();
	size_t							entryCount = envRequest.envExtra.size() + envRequest.envBase.size();
	std::string						temp;
	

	try
	{
		std::map<t_extension, t_path>::const_iterator interpExtension 
		= m_CgiModule.getInterpreters().find(m_curRequestData->getExtension());

		if (interpExtension == m_CgiModule.getInterpreters().end())
			throw std::runtime_error("interpreter not found");

		assert (m_envStr.size() == 0 && m_envPtr.size() == 0 && m_argPtr.size() == 0);

		m_envStr.reserve(entryCount);
		m_envPtr.reserve(entryCount + 1);
		m_argPtr.reserve(3);

		for (size_t i = 0; i < envRequest.envBase.size(); i++)
		{
			temp = (envBase[envRequest.envBase[i].first] + "=" + envRequest.envBase[i].second);
			m_envStr.emplace_back();
			StringUtils::move(m_envStr.back(), temp);
		}
		
		for (t_EnvExtraIter it = envRequest.envExtra.begin(); it != envRequest.envExtra.end(); it++)
		{
			temp = (it->first + "=" + it->second);
			m_envStr.emplace_back();
			StringUtils::move(m_envStr.back(), temp);
		}

		for (size_t i = 0; i < entryCount; i++)
			m_envPtr.push_back(const_cast<char*>(m_envStr[i].c_str()));

		m_envPtr.push_back(NULL);

		m_argPtr.push_back(const_cast<char*>(interpExtension->second.c_str()));
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

/*
	Parent just closes the pipe ends that doesn't use and simply waits for the EventManager
	to inform it that the child process has exited -> Events
*/
void	CgiModule::InternalCgiWorker::mf_executeParent()
{
	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ChildToParent[1]);
	mf_closeFd(m_EmergencyPhone[1]);
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
void	CgiModule::InternalCgiWorker::mf_executeChild()
{
	char EmergencyCode[2];

	::close(m_EmergencyPhone[0]);

	if (::dup2(m_ParentToChild[0], STDIN_FILENO) == -1 ||
		::dup2(m_ChildToParent[1], STDOUT_FILENO) == -1)
	{
		EmergencyCode[0] = E_EMER_DUP2;
		EmergencyCode[1] = errno;
		write(m_EmergencyPhone[1], EmergencyCode, 2);
		mf_closeFd(m_ParentToChild[0]);
		mf_closeFd(m_ChildToParent[1]);
		mf_closeFd(m_EmergencyPhone[1]);
		::close(m_ParentToChild[1]);
		::close(m_ChildToParent[0]);		
		
		// wait to be killed lol
		while (1)
			::usleep(1000);
	}
		
	::close(m_ParentToChild[1]);
	::close(m_ChildToParent[0]);

	::execve(m_argPtr[0], m_argPtr.getArray(), m_envPtr.getArray());

	EmergencyCode[0] = E_EMER_EXECVE;
	EmergencyCode[1] = errno;
	write(m_EmergencyPhone[1], EmergencyCode, 2);
	mf_closeFd(m_ParentToChild[0]);
	mf_closeFd(m_ChildToParent[1]);
	mf_closeFd(m_EmergencyPhone[1]);
	
	//wait to be killed lol
	while (1)
		::usleep(1000);
}





