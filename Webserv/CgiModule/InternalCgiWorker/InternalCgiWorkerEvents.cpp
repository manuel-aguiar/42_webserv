/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorkerEvents.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:43:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/16 18:29:51 by mmaria-d         ###   ########.fr       */
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



void	CgiModule::InternalCgiWorker::mf_EventCallback_onRead(Callback& callback)
{
	InternalCgiWorker* worker = static_cast<InternalCgiWorker*>(callback.getData());
	worker->mf_readScript();
}

void	CgiModule::InternalCgiWorker::mf_EventCallback_onWrite(Callback& callback)
{
	InternalCgiWorker* worker = static_cast<InternalCgiWorker*>(callback.getData());
	worker->mf_writeScript();
}

void	CgiModule::InternalCgiWorker::mf_EventCallback_OnEmergency(Callback& event)
{
	InternalCgiWorker* worker = static_cast<InternalCgiWorker*>(event.getData());
	worker->mf_readEmergencyPhone();
}

void	CgiModule::InternalCgiWorker::mf_readScript()
{
	int triggeredFlags;

	triggeredFlags = m_readEvent.getTriggeredFlags();
	
	if (triggeredFlags & EPOLLIN)
	{
		if (m_curRequestData->UserRead(m_readEvent.getFd()) == 0)
		{
			mf_disableMyEvent(m_readEvent, true);
			if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
				mf_waitChild();
		}
	}
	
	if ((triggeredFlags & (EPOLLERR | EPOLLHUP)) && !(triggeredFlags & EPOLLIN))
	{
		mf_disableMyEvent(m_readEvent, true);
		if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
			mf_waitChild();
	}
}

void	CgiModule::InternalCgiWorker::mf_writeScript()
{
	int triggeredFlags;

	triggeredFlags = m_readEvent.getTriggeredFlags();
	if (triggeredFlags & (EPOLLERR | EPOLLHUP))
	{
		mf_disableMyEvent(m_writeEvent, true);
		if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
			mf_waitChild();
	}

	if (triggeredFlags & EPOLLOUT)
		m_curRequestData->UserWrite(m_readEvent.getFd());

}



/*
	Although unlikely, we may not receive the full emergency message in one go, so we need to keep reading
	until we get the full message. We will only read 2 bytes, so we can be sure that we will get the full

	So, there are various scenarios that can happen:

		1. we read zero bytes (EOF), 0 emergency bytes read so far,  -> successfull exit by the child, cleanclose()
		2. we read zero bytes (EOF), 1 emergency bytes read so far, -> incomplete message, forcedclose()
		3. we read 1 bytes


*/

void	CgiModule::InternalCgiWorker::mf_disableMyEvent(Event& myEvent, bool markAsStale)
{
	if (myEvent.getFd() == -1)
		return ;
	m_CgiModule.mf_accessEventManager().delEvent(myEvent, markAsStale);
	myEvent.setFd(-1);
}

void	CgiModule::InternalCgiWorker::disableAllEvents(bool markAsStale)
{
	mf_disableMyEvent(m_EmergencyEvent, markAsStale);
	mf_disableMyEvent(m_readEvent, markAsStale);
	mf_disableMyEvent(m_writeEvent, markAsStale);
}

void	CgiModule::InternalCgiWorker::mf_readEmergencyPhone()
{
	int		triggeredFlags;
	int		bytesRead;

	triggeredFlags = m_EmergencyEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = ::read(	m_EmergencyEvent.getFd(), 
							&m_EmergencyBuffer[m_EmergencyBytesRead], 
							sizeof(m_EmergencyBuffer) - m_EmergencyBytesRead);

		if (bytesRead == -1)
		{
			std::cout << "emergency stale triggered, fd" << m_EmergencyEvent.getFd() << std::endl;
			return ;
		}
			

		m_EmergencyBytesRead += bytesRead;

		if (m_EmergencyBytesRead == 0)	
			return (mf_disableMyEvent(m_EmergencyEvent));
		return (mf_childFailure());
	}

	if (((triggeredFlags & EPOLLERR) || (triggeredFlags & EPOLLHUP)) && !(triggeredFlags & EPOLLIN))
	{
		if (m_EmergencyBytesRead == 0)	
			return (mf_disableMyEvent(m_EmergencyEvent));
		return (mf_childFailure());
	}
}