

# include "Connection.hpp"
# include "../../Events/Subscription/Subscription.hpp"

namespace Conn
{

	void
	Connection::events_setUser		(const Events::Subscription::User data)
	{
		m_monitor.accessEvent().setUser(data);
	}
	void
	Connection::events_setCallback	(const Events::Subscription::Callback handler)
	{
		m_monitor.accessEvent().setCallback(handler);
	}

	void
	Connection::events_setMonitoredEvents(const Events::Monitor::Mask flags)
	{
		m_monitor.accessEvent().setMonitoredEvents(flags);
	}

	Events::Monitor::Mask
	Connection::events_getTriggeredEvents() const
	{
		return (m_monitor.getEvent().getTriggeredEvents());
	}

	Ws::Sock::fd
	Connection::sock_getFd() const
	{
		return (m_socket.getSockFd());
	}

	const Ws::BindInfo&
	Connection::sock_getBindInfo() const
	{
		return (m_socket.getBindInfo());
	}
}