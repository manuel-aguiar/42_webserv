

// Project Headers
# include "CgiWorker.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../CgiInternalRequest/CgiInternalRequest.hpp"
# include "../../ServerManager/EventManager/EventManager/EventManager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../Globals/Globals.hpp"

namespace Cgi
{

	void	Cgi::Module::Worker::mf_EventCallback_onRead(::Callback& callback)
	{
		Worker* worker = static_cast<Worker*>(callback.getData());
		worker->mf_readScript();
	}

	void	Cgi::Module::Worker::mf_EventCallback_onWrite(::Callback& callback)
	{
		Worker* worker = static_cast<Worker*>(callback.getData());
		worker->mf_writeScript();
	}

	void	Cgi::Module::Worker::mf_EventCallback_OnEmergency(::Callback& event)
	{
		Worker* worker = static_cast<Worker*>(event.getData());
		worker->mf_readEmergencyPhone();
	}

	void	Cgi::Module::Worker::mf_readScript()
	{
		int 				bytesRead;
		Ws::Epoll::Flags 	triggeredFlags;

		triggeredFlags = m_readEvent.getTriggeredFlags();
		//std::cout << "\t\t\tread called" << std::endl;
		
		if (triggeredFlags & Ws::Epoll::READ)
		{
			bytesRead = m_curRequestData->IO_CallTheUser(IO_Callback::READ, m_readEvent.getFd());

			assert(bytesRead != -1);

			if (bytesRead == 0)
			{
				mf_disableCloseMyEvent(m_readEvent, true);
				if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
					mf_waitChild();
			}
		}
		
		if ((triggeredFlags & (Ws::Epoll::ERROR | Ws::Epoll::HANGUP)) && !(triggeredFlags & Ws::Epoll::READ))
		{
			mf_disableCloseMyEvent(m_readEvent, true);
			if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
				mf_waitChild();
		}
	}

	void	Cgi::Module::Worker::mf_writeScript()
	{
		int 				bytesWritten;
		Ws::Epoll::Flags 	triggeredFlags;
		
		triggeredFlags = m_writeEvent.getTriggeredFlags();
		
		//std::cout << "write triggered" << std::endl;
		
		if (triggeredFlags & Ws::Epoll::WRITE)
		{
			bytesWritten = m_curRequestData->IO_CallTheUser(IO_Callback::WRITE, m_writeEvent.getFd());

			assert(bytesWritten != -1);	

			if (bytesWritten == 0)
			{
				mf_disableCloseMyEvent(m_writeEvent, true);
				if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
					mf_waitChild();
			}
		}
		
		if (triggeredFlags & (Ws::Epoll::ERROR | Ws::Epoll::HANGUP))
		{
			mf_disableCloseMyEvent(m_writeEvent, true);
			if (m_writeEvent.getFd() == -1 && m_readEvent.getFd() == -1)
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

	void	Cgi::Module::Worker::mf_disableCloseMyEvent(Event& myEvent, bool markAsStale)
	{
		Ws::fd fd = myEvent.getFd();

		if (fd == -1)
			return ;
		m_CgiModule.mf_accessEventManager().delEvent(myEvent, markAsStale);
		::close(fd);
		myEvent.setFd(-1);
	}

	void	Cgi::Module::Worker::disableCloseAllEvents(bool markAsStale)
	{
		mf_disableCloseMyEvent(m_EmergencyEvent, markAsStale);
		mf_disableCloseMyEvent(m_readEvent, markAsStale);
		mf_disableCloseMyEvent(m_writeEvent, markAsStale);
	}

	void	Cgi::Module::Worker::mf_readEmergencyPhone()
	{
		int		triggeredFlags;
		int		bytesRead;

		triggeredFlags = m_EmergencyEvent.getTriggeredFlags();
		
		if (triggeredFlags & EPOLLIN)
		{


			bytesRead = ::read(	m_EmergencyEvent.getFd(), 
								&m_EmergencyBuffer[m_EmergencyBytesRead], 
								sizeof(m_EmergencyBuffer) - m_EmergencyBytesRead);

			assert(bytesRead != -1);
			if (bytesRead == -1)
			{
				std::cout << " emergency stale event on fd: " << m_EmergencyEvent.getFd() << " " << strerror(errno) << std::endl;
				// a stale read i cannot fix, but apparently is causing no extra problems
				return ;
			}

			m_EmergencyBytesRead += bytesRead;

			if (m_EmergencyBytesRead == 0)
				return (mf_disableCloseMyEvent(m_EmergencyEvent, true));
				
			return (mf_childFailure());
		}

		if ((triggeredFlags & (EPOLLERR | EPOLLHUP)) && !(triggeredFlags & EPOLLIN))
		{
			if (m_EmergencyBytesRead == 0)	
				return (mf_disableCloseMyEvent(m_EmergencyEvent, true));
			return (mf_childFailure());
		}
	}

}; // namespace Cgi