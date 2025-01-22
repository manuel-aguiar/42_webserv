

# include "Subscription.hpp"

// accessors

namespace Events
{
	Manager::Subscription::User				Manager::Subscription::accessUser()
	{
		return (m_user);
	}

	Manager::Subscription::Service			Manager::Subscription::accessService()
	{
		return (m_handler);
	}

	// getters
	Ws::fd						Manager::Subscription::getFd() const
	{
		return (m_fd);
	}

	Events::Monitor::Type			Manager::Subscription::getMonitoredFlags() const
	{
		return (m_monitoredEvents);
	}

	Events::Monitor::Type			Manager::Subscription::getTriggeredFlags() const
	{
		return (m_monitoredEvents);
	}

	// setters

	void	Manager::Subscription::setFd(const Ws::fd fd)
	{
		m_fd = fd;
	}

	void	Manager::Subscription::setMonitoredFlags(const Events::Monitor::Type flags)
	{
		m_monitoredEvents = flags;
	}

	void	Manager::Subscription::setUser(const Manager::Subscription::User user)
	{
		m_user = user;
	}

	void	Manager::Subscription::setService(const Manager::Subscription::Service handler)
	{
		m_handler = handler;
	}

}