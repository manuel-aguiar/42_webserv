

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

void	Worker::mf_readScript()
{
	int 					bytesRead = 0;
	Events::Monitor::Mask 	triggeredEvents;

	triggeredEvents = m_readEvent->getTriggeredEvents();
	//std::cout << "\t\t\tread called" << std::endl;

	// if closed and there is no data
	if ((triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP)) && !(triggeredEvents & Events::Monitor::READ))
		goto disableReadEvent;

	// nothing to read, return
	if (!triggeredEvents & Events::Monitor::READ)
		return ;

	if (m_headerParser.getState() != HeaderData::FINISH)
	{
		// headers didn't fit the buffer, abort
		if (m_headerBuffer.size() == m_headerBuffer.capacity())
		{
			mf_recycleRuntimeFailure();
			return ;
		}

		bytesRead = m_headerBuffer.read(m_readEvent->getFd(), m_headerBuffer.size());
		
		ASSERT_EQUAL(bytesRead != -1, true, "InternalCgiWorker::mf_readScript(): bytesread should never be -1");

		if (bytesRead == 0)
			goto disableReadEvent;

		switch (m_headerParser.parse(m_headerBuffer))
		{
			case HeaderData::FAIL:
				mf_recycleRuntimeFailure();
				return ;
			case HeaderData::PASS:
			{
				// push extra message body to the body buffer for sending later
				if (m_headerParser.getTempBody().size() > 0)
					m_bodyBuffer.push(m_headerParser.getTempBody().data(), m_headerParser.getTempBody().size());
				
				// send status code and headers to the user
				(m_curRequestData->getReceiveHeaders_Callback())(m_curRequestData->getUser(), 
																m_headerParser.getStatusCode(), 
																m_headerParser.getHeaders());
				break ;
			}
			default:
				break ;
		}
	}
	else
	{

	}
	// notify user that body is ready
	


	return ;

	disableReadEvent:

		mf_disableCloseMyEvent(*m_readEvent, true);
		if (m_writeEvent->getFd() == -1 && m_readEvent->getFd() == -1)
			mf_waitChild();	
}

void	Worker::mf_writeScript()
{
	int 					bytesWritten = 0;
	Events::Monitor::Mask 	triggeredEvents;
	Cgi::IO::WriteState 	state;
	
	triggeredEvents = m_writeEvent->getTriggeredEvents();
	
	// error, disable straight away
	if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
		goto disableWriteEvent;
	
	// no write, return
	if (!triggeredEvents & Events::Monitor::WRITE)
		return ;
	
	// there are leftovers in buffer, send those
	if (m_writeBuffer.writeOffset() != m_writeBuffer.size())
	{
		ASSERT_EQUAL(bytesWritten != -1, true, "InternalCgiWorker::mf_writeScript(): ::write shouuld not return -1");

		bytesWritten = m_writeBuffer.write(m_writeEvent->getFd(), m_writeBuffer.writeOffset());
		if (bytesWritten == 0)
			goto disableWriteEvent;
		return ;
	}

	// ask user to give us more data to send
	state = (m_curRequestData->getWriteToScript_Callback())(m_curRequestData->getUser(), m_writeBuffer);
	
	switch (state)
	{
		case Cgi::IO::SEND:
		{
			bytesWritten = m_writeBuffer.write(m_writeEvent->getFd());
			ASSERT_EQUAL(bytesWritten != -1, true, "InternalCgiWorker::mf_writeScript(): ::write shouuld not return -1");
			if (bytesWritten == 0)
				goto disableWriteEvent;
			return ;
		}
		case Cgi::IO::WAIT:
			return ;
		case Cgi::IO::CLOSE:
			goto disableWriteEvent;
	}

	return ;

	disableWriteEvent:

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