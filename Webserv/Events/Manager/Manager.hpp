

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

// Project headers
# include "../m_EventsDefinitions.h"
# include "../../GenericUtils/Webserver_Definitions.h"

//Toolkit headers
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"


class Globals;

namespace Events
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
			void			returnSubscription(Subscription& event);

			int				startMonitoring(Subscription& event, bool markAsStale);
			int				modify(Subscription& event, bool markAsStale);
			int				stopMonitoring(Subscription& event, bool markAsStale);
			
			//getters
			size_t			getMonitoringCount() const;
			
		private:
			class								InternalSubs;
			
			size_t								m_subscribeCount;
			t_fd								m_epollfd;
			Globals&							m_globals;
			t_fd								m_maxStaleFd;

			HeapArray<InternalSubs>				m_subscriptions;
			HeapCircularQueue<InternalSubs*>	m_availableSubs;
			HeapArray<t_byte>					m_staleEvents;
			Events::Monitor::Event				m_epollEvents[MAX_EPOLL_EVENTS];

			void								mf_markFdStale(t_fd fd);
			int									mf_isFdStale(t_fd fd);
			
			Manager(const Manager& copy);
			Manager& operator=(const Manager& assign);
	};

	typedef Manager::Subscription Subscription;
}


#endif
