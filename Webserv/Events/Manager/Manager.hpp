

#ifndef EVENTMANAGER_HPP

# define EVENTMANAGER_HPP

// Project headers
# include "../m_EventsDefinitions.h"
# include "../../GenericUtils/Webserver_Definitions.h"

//Toolkit headers
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"


/// forward declarations ///
class Globals;
class InternalSub;
namespace Events
{
	class Subscription;
}
//////////////////////////


namespace Events
{
	class Manager
	{
		public:
			
			Manager(size_t maxSubscriptions, Globals& globals);
			~Manager();

			//methods
			int				ProcessEvents(int timeOut);

			// acquire and return a subscription
			Subscription*	acquireSubscription();
			void			returnSubscription(Subscription& event);

			// monitor, update and stop monitoring a subscription's Event interests
			int				startMonitoring(Subscription& event, bool markAsStale);
			int				updateEvents(Subscription& event, bool markAsStale);
			int				stopMonitoring(Subscription& event, bool markAsStale);
			
			//getters
			size_t			getMonitoringCount() const;
			
		private:
			size_t								m_subscribeCount;
			t_fd								m_epollfd;
			t_fd								m_maxStaleFd;
			Globals&							m_globals;

			HeapArray<InternalSub>				m_subscriptions;
			HeapCircularQueue<InternalSub*>		m_availableSubs;
			HeapArray<t_byte>					m_staleEvents;
			Events::Monitor::Event				m_epollEvents[MAX_EPOLL_EVENTS];

			void								mf_markFdStale(t_fd fd);
			int									mf_isFdStale(t_fd fd);
			
			Manager(const Manager& copy);
			Manager& operator=(const Manager& assign);
	};
}


#endif
