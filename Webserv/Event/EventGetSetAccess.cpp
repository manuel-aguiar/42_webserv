

# include "Event.hpp"

// getters

Ws::fd						Event::getFd() const
{
	return (m_fd);
}

const Callback&				Event::getCallback() const
{
	return (m_callback);
}

int							Event::getMonitoredFlags() const
{
	return (m_monitoredFlags);
}

int							Event::getTriggeredFlags() const
{
	return (m_monitoredFlags);
}

// setters

void	Event::setFd(const Ws::fd fd)
{
	m_fd = fd;
}

void	Event::setMonitoredFlags(int flags)
{
	m_monitoredFlags = flags;
}

void	Event::setTriggeredFlags(int flags)
{
	m_triggeredFlags = flags;
}

void	Event::setCallback(const Callback::Data data, const Callback::Handler handler)
{
	m_callback.setData(data);
	m_callback.setHandler(handler);
}
