

#ifndef EVENTSUBSCRIPTION_HPP

# define EVENTSUBSCRIPTION_HPP

# include "../Events.h"
# include "../Manager/Manager.hpp"

namespace Events
{
	class Manager::Subscription
	{
		public:

			typedef void* 				User;
			typedef void 				(*Service)(Subscription& event);

			//methods
			void						notifyUser();
			void						reset();

			// accessors
			Subscription::User			accessUser();
			Subscription::Service		accessService();

			//getters
			Ws::fd						getFd()					const;
			Events::Monitor::Type		getMonitoredFlags()		const;
			Events::Monitor::Type		getTriggeredFlags()		const;

			//setters
			void						setFd				(const Ws::fd fd);
			void						setMonitoredFlags	(const Events::Monitor::Type flags);
			void						setUser				(const Subscription::User data);
			void						setService			(const Subscription::Service handler);

			// no setter for triggeredFlags, it is Manager who sets those

			
		protected:
			Ws::fd						m_fd;
			Events::Monitor::Type			m_monitoredEvents;
			Events::Monitor::Type			m_triggeredEvents;
			Subscription::User			m_user;
			Subscription::Service		m_handler;

			Subscription();
			~Subscription();			
			Subscription(const Subscription& copy);
			Subscription& operator=(const Subscription& assign);


			//for internal use

	};
}

#endif
