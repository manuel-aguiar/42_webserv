/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalCgiWorkerEvents.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:43:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/18 10:17:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project Headers
# include "InternalCgiWorker.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../InternalCgiRequestData/InternalCgiRequestData.hpp"
# include "../Test_EventManager/Test_EventManager.hpp"
# include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../../Globals/Globals.hpp"



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
	int bytesRead;
	int triggeredFlags;

	triggeredFlags = m_readEvent.getTriggeredFlags();
	
	if (triggeredFlags & EPOLLIN)
	{
		bytesRead = m_curRequestData->UserRead(m_readEvent.getFd());
		//assert(bytesRead != -1);
		
		if (bytesRead == 0)
		{
			mf_disableMyEvent(m_readEvent);
			if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
			{
				mf_waitChild();
				m_CgiModule.mf_markWorkerForCleanup(*this);
			};
		}
	}
	
	if ((triggeredFlags & (EPOLLERR | EPOLLHUP)) && !(triggeredFlags & EPOLLIN))
	{
		mf_disableMyEvent(m_readEvent);
		if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
		{
			mf_waitChild();
			m_CgiModule.mf_markWorkerForCleanup(*this);
		};
	}
}

void	CgiModule::InternalCgiWorker::mf_writeScript()
{
	int bytesWritten;
	int triggeredFlags;

	triggeredFlags = m_writeEvent.getTriggeredFlags();

	if (triggeredFlags & EPOLLOUT)
	{
		bytesWritten = m_curRequestData->UserWrite(m_writeEvent.getFd());
		//assert(bytesWritten != -1);
		if (bytesWritten == 0)
		{
			mf_disableMyEvent(m_writeEvent);
			if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
			{
				mf_waitChild();
				m_CgiModule.mf_markWorkerForCleanup(*this);
			};
		}
	}

	if (triggeredFlags & (EPOLLERR | EPOLLHUP))
	{
		mf_disableMyEvent(m_writeEvent);
		if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
		{
			mf_waitChild();
			m_CgiModule.mf_markWorkerForCleanup(*this);
		};
	}
}



/*
	Although unlikely, we may not receive the full emergency message in one go, so we need to keep reading
	until we get the full message. We will only read 2 bytes, so we can be sure that we will get the full

	So, there are various scenarios that can happen:

		1. we read zero bytes (EOF), 0 emergency bytes read so far,  -> successfull exit by the child, cleanclose()
		2. we read zero bytes (EOF), 1 emergency bytes read so far, -> incomplete message, forcedclose()
		3. we read 1 bytes


*/

void	CgiModule::InternalCgiWorker::mf_disableMyEvent(Event& myEvent)
{
	if (myEvent.getFd() == -1)
		return ;
	m_CgiModule.mf_accessEventManager().delEvent(myEvent);
	myEvent.setFd(-1);
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
		//assert(bytesRead != -1);
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