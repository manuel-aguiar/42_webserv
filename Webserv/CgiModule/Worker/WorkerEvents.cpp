

// Project Headers
# include "Worker.hpp"
# include "../InternalReq/InternalReq.hpp"
# include "../../Events/Subscription/Subscription.hpp"

// C++ headers
# include <vector>

// C headers
# include <unistd.h>

void	Worker::mf_EventCallback_onRead(Events::Subscription& event)
{
	Worker* worker = static_cast<Worker*>(event.accessUser());
	worker->mf_readScript();
}

void	Worker::mf_EventCallback_onWrite(Events::Subscription& event)
{
	Worker* worker = static_cast<Worker*>(event.accessUser());
	worker->mf_writeScript();
}

void	Worker::mf_EventCallback_OnEmergency(Events::Subscription& event)
{
	Worker* worker = static_cast<Worker*>(event.accessUser());
	worker->mf_readEmergencyPhone();
}

void	Worker::mf_failSendHeaders()
{
	m_headerParser.parse(m_headerBuffer);
	(m_curRequestData->getReceiveHeaders_Callback())(m_curRequestData->getUser(), m_headerParser);
	mf_recycleRuntimeFailure();
}

void	Worker::mf_disableAndWait(Events::Subscription& myEvent)
{
	mf_disableCloseMyEvent(myEvent, true);
	if (m_writeEvent->getFd() == -1 && m_readEvent->getFd() == -1)
		mf_waitChild();
}

void	Worker::mf_readScript()
{
	int 					bytesRead = 0;
	Events::Monitor::Mask 	triggeredEvents;
	
	triggeredEvents = m_readEvent->getTriggeredEvents();

	if ((triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP)) && !(triggeredEvents & Events::Monitor::READ))
	{
		if (m_headerParser.getParsingState() == Cgi::HeaderData::NEED_MORE_DATA)
			return (mf_recycleRuntimeFailure());
		mf_disableAndWait(*m_readEvent);
		(m_curRequestData->getReadBodyFromScript_Callback())(m_curRequestData->getUser(), Ws::FD_NONE);
	}

	if (!(triggeredEvents & Events::Monitor::READ))
		return ;

	if (m_headerParser.getParsingState() == Cgi::HeaderData::NEED_MORE_DATA)
	{
		if (m_headerBuffer.size() == m_headerBuffer.capacity())
			return (mf_recycleRuntimeFailure());

		bytesRead = m_headerBuffer.read(m_readEvent->getFd(), m_headerBuffer.size());
		
		ASSERT_EQUAL(bytesRead != -1, true, "InternalCgiWorker::mf_readScript(): bytesread should never be -1");

		if (bytesRead == 0)
			return (mf_recycleRuntimeFailure());

		Cgi::HeaderData::ParsingState parseStatus = m_headerParser.parse(m_headerBuffer);
		switch (parseStatus)
		{
			case::Cgi::HeaderData::NEED_MORE_DATA:
				return ;
			case::Cgi::HeaderData::FAIL:
				return (mf_recycleRuntimeFailure());
			case::Cgi::HeaderData::PASS:
			{
				(m_curRequestData->getReceiveHeaders_Callback())(m_curRequestData->getUser(), m_headerParser);
				break ;
			}
		}
	}
	else
		(m_curRequestData->getReadBodyFromScript_Callback())(m_curRequestData->getUser(), m_readEvent->getFd());
}

void	Worker::mf_writeScript()
{
	Events::Monitor::Mask 	triggeredEvents;
	Cgi::IO::State 			state;
	
	triggeredEvents = m_writeEvent->getTriggeredEvents();
	
	if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
	{
		(m_curRequestData->getWriteToScript_Callback())(m_curRequestData->getUser(), Ws::FD_NONE);
		return (mf_disableAndWait(*m_writeEvent));
	}
	
	if (!(triggeredEvents & Events::Monitor::WRITE))
		return ;

	state = (m_curRequestData->getWriteToScript_Callback())(m_curRequestData->getUser(), m_writeEvent->getFd());
	
	switch (state)
	{
		case Cgi::IO::CONTINUE:
			return ;
		case Cgi::IO::CLOSE:
			return (mf_disableAndWait(*m_writeEvent));
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

void	Worker::disableCloseAllEvents(bool markAsStale)
{
	mf_disableCloseMyEvent(*m_EmergencyEvent, markAsStale);
	mf_disableCloseMyEvent(*m_readEvent, markAsStale);
	mf_disableCloseMyEvent(*m_writeEvent, markAsStale);
}

void	Worker::mf_readEmergencyPhone()
{
	int		triggeredEvents;
	int		bytesRead;
	
	triggeredEvents = m_EmergencyEvent->getTriggeredEvents();
	
	if (triggeredEvents & EPOLLIN)
	{


		bytesRead = ::read(	m_EmergencyEvent->getFd(), 
							&m_EmergencyBuffer[m_EmergencyBytesRead], 
							sizeof(m_EmergencyBuffer) - m_EmergencyBytesRead);

		ASSERT_EQUAL(bytesRead != -1, true, "InternalCgiWorker::mf_readEmergencyPhone(): read() should never return -1");

		m_EmergencyBytesRead += bytesRead;

		if (m_EmergencyBytesRead == 0)
			return (mf_disableCloseMyEvent(*m_EmergencyEvent, true));
			
		return (mf_childFailure());
	}

	if ((triggeredEvents & (EPOLLERR | EPOLLHUP)) && !(triggeredEvents & EPOLLIN))
	{
		if (m_EmergencyBytesRead == 0)	
			return (mf_disableCloseMyEvent(*m_EmergencyEvent, true));
		return (mf_childFailure());
	}
}
