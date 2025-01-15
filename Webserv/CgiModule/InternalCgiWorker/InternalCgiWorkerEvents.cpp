/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorkerEvents.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:43:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/15 14:28:00 by mmaria-d         ###   ########.fr       */
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

void	CgiModule::InternalCgiWorker::mf_interpretAndKill()
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

	mf_KillWaitChild();
	
	m_curRequestData->CallTheUser(E_CGI_ON_ERROR_RUNTIME);
}


void	CgiModule::InternalCgiWorker::mf_disableEmergencyEvent()
{
	if (m_EmergencyEvent.getFd() == -1)
		return ;
	m_curRequestData->accessEventManager()->delEvent(m_EmergencyEvent);
	m_EmergencyEvent.setFd(-1);
}

void	CgiModule::InternalCgiWorker::mf_readEmergencyPhone()
{
	int		triggeredFlags;
	int		bytesRead;

	triggeredFlags = m_EmergencyEvent.getTriggeredFlags();
	
	//if (m_EmergencyEvent.getFd() == -1)
	//	return ;

	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(	m_EmergencyEvent.getFd(), 
							&m_EmergencyBuffer[m_EmergencyBytesRead], 
							sizeof(m_EmergencyBuffer) - m_EmergencyBytesRead);

		// outdated event, ignore, wait for epoll to trigger for the new file attached to this fd
		//if (bytesRead == -1)
		//	return ;

		m_EmergencyBytesRead += bytesRead;

		// treat the event first, analyse afterwards
		//if (bytesRead == 0 || m_EmergencyBytesRead == 2)
		//	mf_disableEmergencyEvent();

		//nothing after read, all good
		if (m_EmergencyBytesRead == 0)	
			return (mf_JustWaitChild());

		// full read or partial read but EOF received
		if (bytesRead == 0 || m_EmergencyBytesRead == 2)
			return (mf_interpretAndKill());
	}

	if ((triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP))
	{
		//mf_disableEmergencyEvent();
		if (m_EmergencyBytesRead != 0)
			return (mf_interpretAndKill());
		mf_JustWaitChild();
	}
}