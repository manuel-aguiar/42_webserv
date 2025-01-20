

#ifndef EVENT_HPP

# define EVENT_HPP

# include "../GenericUtils/Webserver_Definitions.h"
# include "../Callback/Callback.hpp"

class Event
{
	public:
		Event();
		~Event();

		//methods
		void		handle();
		void		reset();

		//getters
		Ws::fd						getFd()					const;
		Ws::Epoll::Flags			getMonitoredFlags()		const;
		Ws::Epoll::Flags			getTriggeredFlags()		const;
		const Callback&				getCallback()			const;
		
		//setters
		void						setFd				(const Ws::fd fd);
		void						setMonitoredFlags	(Ws::Epoll::Flags flags);
		void						setTriggeredFlags	(Ws::Epoll::Flags flags);
		void						setCallback			(const Callback::Data data, const Callback::Handler handler);

	protected:
		Ws::fd						m_fd;
		Ws::Epoll::Flags			m_monitoredFlags;
		Ws::Epoll::Flags			m_triggeredFlags;
		Callback					m_callback;
		
		Event(const Event& copy);
		Event& operator=(const Event& assign);


		//for internal use
		Ws::fd						m_subscribedFd;
		Ws::Epoll::Flags			m_subscribedFlags;
};

#endif
