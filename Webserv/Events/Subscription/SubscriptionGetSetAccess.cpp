

# include "Subscription.hpp"

// accessors

namespace Events
{
	Manager::Subscription::User				Manager::Subscription::accessUser()
	{
		return (m_user);
	}

	Manager::Subscription::Callback			Manager::Subscription::accessService()
	{
		return (m_callback);
	}

	// getters
	Ws::fd						Manager::Subscription::getFd() const
	{
		return (m_fd);
	}

	Events::Monitor::Mask			Manager::Subscription::getMonitoredEvents() const
	{
		return (m_monitoredEvents);
	}

	Events::Monitor::Mask			Manager::Subscription::getTriggeredEvents() const
	{
		return (m_monitoredEvents);
	}

	// setters

	void	Manager::Subscription::setFd(const Ws::fd fd)
	{
		m_fd = fd;
	}

	void	Manager::Subscription::setMonitoredEvents(const Events::Monitor::Mask flags)
	{
		m_monitoredEvents = flags;
	}

	void	Manager::Subscription::setUser(const Manager::Subscription::User user)
	{
		m_user = user;
	}

	void	Manager::Subscription::setCallback(const Manager::Subscription::Callback handler)
	{
		m_callback = handler;
	}

}