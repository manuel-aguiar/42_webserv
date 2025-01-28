

#ifndef EVENTSUBSCRIPTION_HPP

# define EVENTSUBSCRIPTION_HPP

# include "../m_EventsDefinitions.h"

namespace Events
{
	class Subscription
	{
		public:
			Subscription();
			~Subscription();			
			Subscription(const Subscription& copy);
			Subscription& operator=(const Subscription& assign);

			typedef void* 				User;
			typedef void 				(*Callback)(Subscription& event);

			//methods
			void						notify();
			void						reset();

			// accessors
			Subscription::User			accessUser();
			Subscription::Callback		accessCallback();

			//getters
			Ws::fd						getFd()					const;
			Events::Monitor::Mask		getMonitoredEvents()	const;
			Events::Monitor::Mask		getTriggeredEvents()	const;

			//setters
			void						setFd				(const Ws::fd fd);
			void						setMonitoredEvents	(const Events::Monitor::Mask flags);
			void						setUser				(const Subscription::User data);
			void						setCallback			(const Subscription::Callback handler);

			// no setter for triggeredFlags, it is Manager who sets those

			
		protected:

			typedef enum
			{
				SUBSCRIBED,
				UNSUBSCRIBED
			}	State;

			Ws::fd						m_fd;
			Events::Monitor::Mask		m_monitoredEvents;
			Events::Monitor::Mask		m_triggeredEvents;
			Subscription::User			m_user;
			Subscription::Callback		m_callback;

	};
}

#endif
