

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

# include "../../GenericUtils/Webserver_Definitions.h"

class Globals;
class EventCallback;

# define MAX_EPOLL_FDS 1000000

class EventManager
{
	public:
		EventManager(Globals& globals);
		~EventManager();

		//methods
		int				ProcessEvents(int timeOut);

		int				addEvent(EventCallback& event, bool markAsStale = true);
		int				modEvent(EventCallback& event, bool markAsStale = true);
		int				delEvent(EventCallback& event, bool markAsStale = true);
		
		//getters
		size_t			getSubscribeCount() const;
		
	private:
		class			InternalEvent;
		
		size_t			m_subscribeCount;
		t_fd			m_epollfd;
		Globals&		m_globals;
		
		t_fd			m_maxStaleFd;	
		t_epoll_event	m_events[MAX_EPOLL_EVENTS];
		t_byte			m_staleEvents[(MAX_EPOLL_FDS / 8) + 1];

		void			mf_markFdStale(t_fd fd);
		int				mf_isFdStale(t_fd fd);
		
		EventManager(const EventManager& copy);
		EventManager& operator=(const EventManager& assign);
};



#endif
