

# include "InternalListener.hpp"
# include "../../Events/Subscription/Subscription.hpp"

# include <iostream>


int		InternalListener::open()
{
	int res = m_listener.open();
	
	if (!res)
		return (0);

	m_monitor.acquire();
	Events::Subscription& event = m_monitor.accessEvent();

	event.setFd(m_listener.getConnInfo().sockfd);
	event.setMonitoredEvents(Events::Monitor::READ 
							| Events::Monitor::ERROR 
							| Events::Monitor::HANGUP 
							| Events::Monitor::EDGE_TRIGGERED);
	event.setUser(this);
	event.setCallback(EventCallbackAccept);
	m_monitor.subscribe(false);
	return (1);
}

void	InternalListener::close()
{
	m_monitor.release();
	m_listener.close();
}

void InternalListener::EventCallbackAccept(Events::Subscription& callback)
{
	InternalListener*	listener;

	listener = reinterpret_cast<InternalListener*>(callback.accessUser());
	listener->accept();
}