

#ifndef EVENTSUBSCRIPTION_HPP

# define EVENTSUBSCRIPTION_HPP

# include "../Event.h"

namespace Event
{
	class Subscription
	{
		public:

			typedef void* 				User;
			typedef void 				(*Handler)(Subscription& event);

			Subscription();
			~Subscription();

			//methods
			void						notifyUser();
			void						reset();

			// accessors
			Subscription::User			accessUser();
			Subscription::Handler		accessHandler();

			//getters
			Ws::fd						getFd()						const;
			Event::Flags::Type			getMonitoredEvents()		const;
			Event::Flags::Type			getTriggeredEvents()		const;

			//setters
			void						setFd				(const Ws::fd fd);
			void						setMonitoredEvents	(const Event::Flags::Type flags);
			void						setUser				(const Subscription::User data);
			void						setHandler			(const Subscription::Handler handler);
			void						setUserHandler		(const Subscription::User data, const Subscription::Handler handler);	
			// no setter for triggeredFlags, it is Manager who sets those

			
		protected:
			Ws::fd						m_fd;
			Event::Flags::Type			m_monitoredEvents;
			Event::Flags::Type			m_triggeredEvents;
			Subscription::User			m_user;
			Subscription::Handler		m_handler;
			
			Subscription(const Subscription& copy);
			Subscription& operator=(const Subscription& assign);


			//for internal use

	};
}

#endif
