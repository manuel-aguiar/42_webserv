

# include "InternalListen.hpp"
# include "../../Events/Subscription/Subscription.hpp"


int		InternalListen::open()
{
	int res = m_listener.open(m_socket);
	
	if (!res)
		// failed to open
		return (0);

	m_monitor.acquire(mf_accessEventManager());
	Events::Subscription* event = m_monitor.accessEvent();

	event->setFd(m_socket.getSockFd());
	event->setMonitoredEvents(Events::Monitor::READ 
							| Events::Monitor::ERROR 
							| Events::Monitor::HANGUP 
							| Events::Monitor::EDGE_TRIGGERED);
	event->setUser(this);
	event->setCallback(EventCallbackAccept);
	m_monitor.subscribe(mf_accessEventManager(), true);

	return (1);
}

void	InternalListen::close()
{
	m_monitor.reset(mf_accessEventManager(), false);
	m_monitor.release(mf_accessEventManager());
	if (m_socket.getSockFd() != -1)
		m_listener.close(m_socket);
}

void InternalListen::EventCallbackAccept(Events::Subscription& callback)
{
	InternalListen*	listener;

	listener = reinterpret_cast<InternalListen*>(callback.accessUser());
	listener->accept();
}