

# include "Subscription.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"
// accessors

namespace Events
{
	Subscription::User
	Subscription::accessUser()
	{
		return (m_user);
	}

	Subscription::Callback
	Subscription::accessCallback()
	{
		return (m_callback);
	}

	// getters
	Ws::fd
	Subscription::getFd() const
	{
		return (m_fdmask.getFd());
	}

	Events::Monitor::Mask
	Subscription::getMonitoredEvents() const
	{
		return (m_monitoredEvents);
	}

	Events::Monitor::Mask
	Subscription::getTriggeredEvents() const
	{
		return (m_triggeredEvents);
	}

	// setters

	void
	Subscription::setFd(const Ws::fd fd)
	{
		m_fdmask.setFd(fd);
	}

	void
	Subscription::setMonitoredEvents(const Events::Monitor::Mask flags)
	{
		#ifndef NDEBUG
			const int validFlags = 0 	|	Events::Monitor::NONE
										| 	Events::Monitor::READ 
										| 	Events::Monitor::WRITE 
										| 	Events::Monitor::ERROR 
										| 	Events::Monitor::HANGUP 
										| 	Events::Monitor::RDHANGUP 
										| 	Events::Monitor::EDGE_TRIGGERED;
		#endif
		ASSERT_EQUAL((flags & ~validFlags) == 0, true, "Subscription::setMonitoredEvents: invalid flag combination");
		
		m_monitoredEvents = flags;
	}

	void
	Subscription::setUser(const Subscription::User user)
	{
		m_user = user;
	}

	void
	Subscription::setCallback(const Subscription::Callback handler)
	{
		m_callback = handler;
	}

	bool
	Subscription::isSubscribed() const
	{
		return (m_fdmask.getState() == Subscription::SUBSCRIBED);
	}
}
