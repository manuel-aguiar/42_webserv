

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

void	Worker::mf_readScript()
{
	int 					bytesRead = 0;
	Events::Monitor::Mask 	triggeredEvents;
	Cgi::IO::State 			state;
	
	triggeredEvents = m_readEvent->getTriggeredEvents();
	////std::cout << "\t\t\tread called" << std::endl;

	if ((triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP)) && !(triggeredEvents & Events::Monitor::READ))
	{
		if (m_headerParser.getParsingState() != Cgi::HeaderData::FINISH)
		{
			//std::cout << "not finished, failing" << std::endl;
			return (mf_failSendHeaders());
		}
		else
			(m_curRequestData->getReadBodyFromScript_Callback())(m_curRequestData->getUser(), Ws::FD_NONE);
		goto disableReadEvent;
	}

	if (!(triggeredEvents & Events::Monitor::READ))
		return ;

	if (m_headerParser.getParsingState() != Cgi::HeaderData::FINISH)
	{
		// headers didn't fit the buffer, inform the user of failure and abort
		if (m_headerBuffer.size() == m_headerBuffer.capacity())
		{
			//std::cout << "buffer full failing" << std::endl;
			return (mf_failSendHeaders());
		}

		bytesRead = m_headerBuffer.read(m_readEvent->getFd(), m_headerBuffer.size());
		
		ASSERT_EQUAL(bytesRead != -1, true, "InternalCgiWorker::mf_readScript(): bytesread should never be -1");

		if (bytesRead == 0)
		{
			//std::cout << "eof without headers finished, failing" << std::endl;
			return (mf_failSendHeaders());
		}

		Cgi::HeaderData::Status headerStatus = m_headerParser.parse(m_headerBuffer);
		//std::cout << "header status: " << headerStatus << std::endl;
		if (headerStatus != Cgi::HeaderData::NEED_MORE_DATA)
		{
			state = (m_curRequestData->getReceiveHeaders_Callback())(m_curRequestData->getUser(), m_headerParser);
			if (headerStatus == Cgi::HeaderData::FAIL)
			{
				//std::cout << "header failed" << std::endl;
				mf_recycleRuntimeFailure();
				return ;
			}
			if (state == Cgi::IO::CLOSE)
				goto disableReadEvent;
		}
	}
	else
	{
		// notify user that body is ready
		// more body data to be read
		state = (m_curRequestData->getReadBodyFromScript_Callback())(m_curRequestData->getUser(), m_readEvent->getFd());
		if (state == Cgi::IO::CLOSE)
			goto disableReadEvent;
	}


	return ;

	disableReadEvent:
		std::cout << "read disabled" << std::endl;
		mf_disableCloseMyEvent(*m_readEvent, true);
		if (m_writeEvent->getFd() == -1 && m_readEvent->getFd() == -1)
			mf_waitChild();	
}

void	Worker::mf_writeScript()
{
	Events::Monitor::Mask 	triggeredEvents;
	Cgi::IO::State 			state;
	
	////std::cout << "\t\t\twrite called" << std::endl;

	triggeredEvents = m_writeEvent->getTriggeredEvents();
	
	// error, disable straight away
	if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
	{
		// trick.... tell the user to write, they fail, and start ignoring
		(m_curRequestData->getWriteToScript_Callback())(m_curRequestData->getUser(), Ws::FD_NONE);
		goto disableWriteEvent;
	}
	
	// no write, return
	if (!(triggeredEvents & Events::Monitor::WRITE))
		return ;

	// ask the user to write and report on what it needs
	state = (m_curRequestData->getWriteToScript_Callback())(m_curRequestData->getUser(), m_writeEvent->getFd());
	
	switch (state)
	{
		case Cgi::IO::CONTINUE:
			return ;
		case Cgi::IO::CLOSE:
		{
			////std::cout << "received indication to close" << std::endl;
			goto disableWriteEvent;
		}
	}

	return ;

	disableWriteEvent:
		////std::cout << "write disabled" << std::endl;
		mf_disableCloseMyEvent(*m_writeEvent, true);
		if (m_writeEvent->getFd() == -1 && m_readEvent->getFd() == -1)
			mf_waitChild();
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
