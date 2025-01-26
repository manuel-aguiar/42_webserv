

// Project Headers
# include "CgiWorker.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Events/Manager/Manager.hpp"

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
	int 					bytesRead;
	Events::Monitor::Mask 	triggeredEvents;

	triggeredEvents = m_readEvent->getTriggeredEvents();
	//std::cout << "\t\t\tread called" << std::endl;
	
	if (triggeredEvents & Events::Monitor::READ)
	{
		bytesRead = m_curRequestData->IO_CallTheUser(Cgi::IO::READ, m_readEvent->getFd());

		assert(bytesRead != -1);

		if (bytesRead == 0)
		{
			mf_disableCloseMyEvent(*m_readEvent, true);
			if (m_writeEvent->getFd() == -1 && m_readEvent->getFd() == -1)
				mf_waitChild();
		}
	}
	
	if ((triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP)) && !(triggeredEvents & Events::Monitor::READ))
	{
		mf_disableCloseMyEvent(*m_readEvent, true);
		if (m_writeEvent->getFd() == -1 && m_readEvent->getFd() == -1)
			mf_waitChild();
	}
}

void	Worker::mf_writeScript()
{
	int 				bytesWritten;
	Events::Monitor::Mask 	triggeredEvents;
	
	triggeredEvents = m_writeEvent->getTriggeredEvents();
	
	//std::cout << "write triggered" << std::endl;
	
	if (triggeredEvents & Events::Monitor::WRITE)
	{
		bytesWritten = m_curRequestData->IO_CallTheUser(Cgi::IO::WRITE, m_writeEvent->getFd());

		assert(bytesWritten != -1);	

		if (bytesWritten == 0)
		{
			mf_disableCloseMyEvent(*m_writeEvent, true);
			if (m_writeEvent->getFd() == -1 && m_readEvent->getFd() == -1)
				mf_waitChild();
		}
	}
	
	if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
	{
		mf_disableCloseMyEvent(*m_writeEvent, true);
		if (m_writeEvent->getFd() == -1 && m_readEvent->getFd() == -1)
			mf_waitChild();
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

void	Worker::mf_disableCloseMyEvent(Events::Subscription& myEvent, bool markAsStale)
{
	Ws::fd fd = myEvent.getFd();

	if (fd == -1)
		return ;
	m_CgiModule.mf_accessEventManager().stopMonitoring(myEvent, markAsStale);
	::close(fd);
	myEvent.setFd(-1);
}

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

		assert(bytesRead != -1);
		if (bytesRead == -1)
		{
			std::cout << " emergency stale event on fd: " << m_EmergencyEvent->getFd() << " " << strerror(errno) << std::endl;
			// a stale read i cannot fix, but apparently is causing no extra problems
			return ;
		}

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