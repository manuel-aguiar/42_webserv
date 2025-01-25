

# include "Subscription.hpp"

// accessors

namespace Events
{
	Subscription::User				Subscription::accessUser()
	{
		return (m_user);
	}

	Subscription::Callback			Subscription::accessService()
	{
		return (m_callback);
	}

	// getters
	Ws::fd						Subscription::getFd() const
	{
		return (m_fd);
	}

	Events::Monitor::Mask			Subscription::getMonitoredEvents() const
	{
		return (m_monitoredEvents);
	}

	Events::Monitor::Mask			Subscription::getTriggeredEvents() const
	{
		return (m_monitoredEvents);
	}

	// setters

	void	Subscription::setFd(const Ws::fd fd)
	{
		m_fd = fd;
	}

	void	Subscription::setMonitoredEvents(const Events::Monitor::Mask flags)
	{
		m_monitoredEvents = flags;
	}

	void	Subscription::setUser(const Subscription::User user)
	{
		m_user = user;
	}

	void	Subscription::setCallback(const Subscription::Callback handler)
	{
		m_callback = handler;
	}

}