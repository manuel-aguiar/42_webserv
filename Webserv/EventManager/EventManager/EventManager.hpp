

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

class Globals;
class Subscription;

# define MAX_EPOLL_FDS 1000000

namespace Event
{
	class Manager
	{
		public:
			class Subscription;

			Manager(size_t maxSubscriptions, Globals& globals);
			~Manager();

			//methods
			int				ProcessEvents(int timeOut);

			Subscription*	acquireSubscription();

			int				add(Subscription& event, bool markAsStale);
			int				modify(Subscription& event, bool markAsStale);
			int				remove(Subscription& event, bool markAsStale);
			
			//getters
			size_t			getMonitoringCount() const;
			
		private:
			class			Validation;
			
			size_t			m_subscribeCount;
			t_fd			m_epollfd;
			Globals&		m_globals;
			
			t_fd			m_maxStaleFd;	
			t_epoll_event	m_events[MAX_EPOLL_EVENTS];
			t_byte			m_staleEvents[(MAX_EPOLL_FDS / 8) + 1];

			void			mf_markFdStale(t_fd fd);
			int				mf_isFdStale(t_fd fd);
			
			Manager(const Manager& copy);
			Manager& operator=(const Manager& assign);
	};
}


#endif
