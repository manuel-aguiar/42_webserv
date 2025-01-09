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

	If any syscall fails, we should let the caller know there was an error, cancel the CgiRequest and reset it
	reset the InternalCgiWorker and return

*/


void   CgiModule::InternalCgiWorker::execute(InternalCgiRequestData& request)
{
	m_curRequestData = &request;

    if (::pipe(m_ParentToChild) == -1 ||
		::pipe(m_ChildToParent) == -1 ||
		::pipe(m_EmergencyPhone) == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), pipe(): " + std::string(strerror(errno)));
		goto ErrorHandler;
    }
	if (!FileDescriptor::setNonBlocking(m_ParentToChild[0]) ||
		!FileDescriptor::setNonBlocking(m_ParentToChild[1]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[0]) ||
		!FileDescriptor::setNonBlocking(m_ChildToParent[1])||
		!FileDescriptor::setNonBlocking(m_EmergencyPhone[0]) ||
		!FileDescriptor::setNonBlocking(m_EmergencyPhone[1]))
	{
		m_globals.logError("InternalCgiWorker::execute(), fcntl(): " + std::string(strerror(errno)));
		goto ErrorHandler;
	}

	if (!mf_prepareExecve())
		goto ErrorHandler;

	m_curRequestData->setReadFd(m_ChildToParent[0]);
	m_curRequestData->setWriteFd(m_ParentToChild[1]);
	m_EmergencyEvent.setFd(m_EmergencyPhone[0]);

	//std::cout << "read fd: " << m_curRequestData->getReadFd() << std::endl;
	//std::cout << "write fd: " << m_curRequestData->getWriteFd() << std::endl;
	//std::cout << "emergency fd: " << m_EmergencyPhone[0] << std::endl;

	mf_CallTheUser(E_CGI_ON_EXECUTE);



	m_curRequestData->accessEventManager()->addEvent(m_EmergencyEvent);

    m_pid = ::fork();
    if (m_pid == -1)
	{
		m_globals.logError("InternalCgiWorker::execute(), fork(): " + std::string(strerror(errno)));
		goto ErrorHandler;
    }
    if (m_pid == 0)
		mf_executeChild();
	else
		mf_executeParent();
	
	return ;

	ErrorHandler:
		mf_CallTheUser(E_CGI_ON_ERROR_STARTUP);
		m_CgiModule.cancelRequest(*m_curRequestData);

}

void	CgiModule::InternalCgiWorker::mf_CallTheUser(const e_CgiCallback event)
{
	Callback& callback = m_curRequestData->accessCallback(event);

	if (callback.getHandler() == NULL)
		return ;
	callback.execute();
}

bool	CgiModule::InternalCgiWorker::mf_prepareExecve()
{
	typedef std::map<t_CgiEnvKey, t_CgiEnvValue>::const_iterator
									t_EnvExtraIter;

	assert(m_curRequestData->accessEventManager() != NULL);

	const t_CgiRequestEnv& 			envRequest = m_curRequestData->getEnvVars();
	const t_CgiEnvKey*				envBase = m_CgiModule.getBaseEnvKeys();
	size_t							entryCount = envRequest.envExtra.size() + envRequest.envBase.size();
	std::string						temp;
	
	try
	{
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
		::exit(EXIT_FAILURE);
	}
		
	::close(m_ParentToChild[1]);
	::close(m_ChildToParent[0]);

	//std::cerr << "checking execve"  << std::endl;

	::execve(m_argPtr[0], m_argPtr.getArray(), m_envPtr.getArray());

	//std::cerr << "execve failed" << std::endl;

	EmergencyCode[0] = E_EMER_EXECVE;
	EmergencyCode[1] = errno;
	write(m_EmergencyPhone[1], EmergencyCode, 2); 
	::exit(EXIT_FAILURE);
}





void	CgiModule::InternalCgiWorker::mf_EventCallback_OnEmergency(Callback& event)
{
	InternalCgiWorker* worker = static_cast<InternalCgiWorker*>(event.getData());
	worker->mf_readEmergencyPipe();
}


/*
	Although unlikely, we may not receive the full emergency message in one go, so we need to keep reading
	until we get the full message. We will only read 2 bytes, so we can be sure that we will get the full

	So, there are various scenarios that can happen:

		1. we read zero bytes (EOF), 0 emergency bytes read so far,  -> successfull exit by the child, cleanclose()
		2. we read zero bytes (EOF), 1 emergency bytes read so far, -> incomplete message, forcedclose()
		3. we read 1 bytes


*/
void	CgiModule::InternalCgiWorker::mf_readEmergencyPipe()
{
	int		triggeredFlags;
	int		bytesRead;

	triggeredFlags = m_EmergencyEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(	m_EmergencyEvent.getFd(), 
							&m_EmergencyBuffer[m_EmergencyBytesRead], 
							sizeof(m_EmergencyBuffer) - m_EmergencyBytesRead);

		m_EmergencyBytesRead += bytesRead;

		switch (bytesRead)
		{
			case 0:
			{
				switch (m_EmergencyBytesRead)
				{
					case 0:
						m_curRequestData->accessEventManager()->delEvent(m_EmergencyEvent);
						return (mf_JustWaitChild());
					case 1:

						m_curRequestData->accessEventManager()->delEvent(m_EmergencyEvent);
						mf_CallTheUser(E_CGI_ON_ERROR_RUNTIME);
						return (forcedClose());
				}
				return ;
			}

			case 2:
			{
				m_curRequestData->accessEventManager()->delEvent(m_EmergencyEvent);
				if (m_EmergencyBuffer[0] == E_EMER_DUP2)
					m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), dup2(): "
					+ std::string(strerror(m_EmergencyBuffer[1])));

				else if (m_EmergencyBuffer[0] == E_EMER_EXECVE)
					m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), execve(): "
					+ std::string(strerror(m_EmergencyBuffer[1])));
				mf_CallTheUser(E_CGI_ON_ERROR_RUNTIME);
				return (forcedClose());
			}

			case 1:
			{
				if (m_EmergencyBytesRead == 2)
				{
					m_curRequestData->accessEventManager()->delEvent(m_EmergencyEvent);
					if (m_EmergencyBuffer[0] == E_EMER_DUP2)
						m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), dup2(): "
						+ std::string(strerror(m_EmergencyBuffer[1])));

					else if (m_EmergencyBuffer[0] == E_EMER_EXECVE)
						m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), execve(): "
						+ std::string(strerror(m_EmergencyBuffer[1])));
					mf_CallTheUser(E_CGI_ON_ERROR_RUNTIME);
					return (forcedClose());
				}
			}
		}
	}

	if ((triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP))
	{
		if (m_EmergencyBytesRead == 1)
		{
			if (m_EmergencyBuffer[0] == E_EMER_DUP2)
				m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), dup2(): inconclusive error");
			else if (m_EmergencyBuffer[0] == E_EMER_EXECVE)
				m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), execve(): inconclusive error");

			mf_CallTheUser(E_CGI_ON_ERROR_RUNTIME);
		}
			
		return (forcedClose());
	}
}