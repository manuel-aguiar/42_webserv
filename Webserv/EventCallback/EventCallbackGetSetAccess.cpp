

# include "EventCallback.hpp"

// accessors


EventCallback::User				EventCallback::accessUser()
{
	return (m_user);
}

EventCallback::Handler			EventCallback::accessHandler()
{
	return (m_handler);
}

// getters

Ws::fd						EventCallback::getFd() const
{
	return (m_fd);
}

Ws::Epoll::Events			EventCallback::getMonitoredEvents() const
{
	return (m_monitoredEvents);
}

Ws::Epoll::Events			EventCallback::getTriggeredEvents() const
{
	return (m_monitoredEvents);
}

// setters

void	EventCallback::setFd(const Ws::fd fd)
{
	m_fd = fd;
}

void	EventCallback::setMonitoredEvents(const Ws::Epoll::Events flags)
{
	m_monitoredEvents = flags;
}

void	EventCallback::setUser(const EventCallback::User user)
{
	m_user = user;
}

void	EventCallback::setHandler(const EventCallback::Handler handler)
{
	m_handler = handler;
}

void	EventCallback::setUserHandler(const EventCallback::User user, const EventCallback::Handler handler)
{
	m_user = user;
	m_handler = handler;
}