/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorkerEvents.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:43:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 23:42:50 by mmaria-d         ###   ########.fr       */
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


void	CgiModule::InternalCgiWorker::mf_EventCallback_OnEmergency(Callback& event)
{
	InternalCgiWorker* worker = static_cast<InternalCgiWorker*>(event.getData());
	worker->mf_readEmergencyPhone();
}


/*
	Although unlikely, we may not receive the full emergency message in one go, so we need to keep reading
	until we get the full message. We will only read 2 bytes, so we can be sure that we will get the full

	So, there are various scenarios that can happen:

		1. we read zero bytes (EOF), 0 emergency bytes read so far,  -> successfull exit by the child, cleanclose()
		2. we read zero bytes (EOF), 1 emergency bytes read so far, -> incomplete message, forcedclose()
		3. we read 1 bytes


*/
void	CgiModule::InternalCgiWorker::mf_readEmergencyPhone()
{
	int		triggeredFlags;
	int		bytesRead;

	triggeredFlags = m_EmergencyEvent.getTriggeredFlags();

	//std::cout << "module read emergency pipe" << std::endl;

	if (m_EmergencyEvent.getFd() == -1)
		return ;		//finish was called already, but this event was already registered by the EventManager

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
						//std::cout << "module, Child exit success, unsubscribe emergency, fd " << m_EmergencyEvent.getFd() << std::endl;
						mf_disableEmergencyEvent();
						return (mf_JustWaitChild());
					case 1:
						//std::cout << "module, Incomplete exit, unsubscribe emergency, fd " << m_EmergencyEvent.getFd() << std::endl;
						mf_disableEmergencyEvent();
						m_curRequestData->CallTheUser(E_CGI_ON_ERROR_RUNTIME);
						return (stopExecution());
				}
				return ;
			}

			case 2:
			{
				//std::cout << "module, Bad exit, unsubscribe emergency, fd " << m_EmergencyEvent.getFd() << std::endl;
				mf_disableEmergencyEvent();
				if (m_EmergencyBuffer[0] == E_EMER_DUP2)
					m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), dup2(): "
					+ std::string(strerror(m_EmergencyBuffer[1])));

				else if (m_EmergencyBuffer[0] == E_EMER_EXECVE)
					m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), execve(): "
					+ std::string(strerror(m_EmergencyBuffer[1])));
				m_curRequestData->CallTheUser(E_CGI_ON_ERROR_RUNTIME);
				return (stopExecution());
			}

			case 1:
			{
				if (m_EmergencyBytesRead == 2)
				{
					//std::cout << "module, Bad exit, double read unsubscribe emergency, fd " << m_EmergencyEvent.getFd() << std::endl;
					mf_disableEmergencyEvent();
					if (m_EmergencyBuffer[0] == E_EMER_DUP2)
						m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), dup2(): "
						+ std::string(strerror(m_EmergencyBuffer[1])));

					else if (m_EmergencyBuffer[0] == E_EMER_EXECVE)
						m_globals.logError("InternalCgiWorker::mf_readEmergencyPipe(), execve(): "
						+ std::string(strerror(m_EmergencyBuffer[1])));
					m_curRequestData->CallTheUser(E_CGI_ON_ERROR_RUNTIME);
					return (stopExecution());
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

			m_curRequestData->CallTheUser(E_CGI_ON_ERROR_RUNTIME);
		}
			
		return (stopExecution());
	}
}

void	CgiModule::InternalCgiWorker::mf_disableEmergencyEvent()
{
	if (m_EmergencyEvent.getFd() != -1)
	{
		m_curRequestData->accessEventManager()->delEvent(m_EmergencyEvent);
		m_EmergencyEvent.setFd(-1);
	}
}