

#ifndef EVENTCALLBACK_HPP

# define EVENTCALLBACK_HPP

# include "../GenericUtils/Webserver_Definitions.h"

class EventCallback
{
	public:

		typedef void* 				User;
		typedef void 				(*Handler)(EventCallback& event);

		EventCallback();
		~EventCallback();

		//methods
		void						handle();
		void						reset();

		// accessors
		EventCallback::User			accessUser();
		EventCallback::Handler		accessHandler();

		//getters
		Ws::fd						getFd()						const;
		Ws::Epoll::Events			getMonitoredEvents()		const;
		Ws::Epoll::Events			getTriggeredEvents()		const;

		//setters
		void						setFd				(const Ws::fd fd);
		void						setMonitoredEvents	(const Ws::Epoll::Events flags);
		void						setUser				(const EventCallback::User data);
		void						setHandler			(const EventCallback::Handler handler);
		void						setUserHandler		(const EventCallback::User data, const EventCallback::Handler handler);	
		// no setter for triggeredFlags, it is EventManager who sets those

		
	protected:
		Ws::fd						m_fd;
		Ws::Epoll::Events			m_monitoredEvents;
		Ws::Epoll::Events			m_triggeredEvents;
		EventCallback::User			m_user;
		EventCallback::Handler		m_handler;
		
		EventCallback(const EventCallback& copy);
		EventCallback& operator=(const EventCallback& assign);


		//for internal use
		Ws::fd						m_subscribedFd;
		Ws::Epoll::Events			m_subscribedEvents;
};

#endif
