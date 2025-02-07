

# include "ListeningSocket.hpp"
# include "../../Events/Subscription/Subscription.hpp"

# include <iostream>


int		ListeningSocket::open()
{
	int res = m_listener.open();
	
	if (!res)
		return (0);

	m_monitor.acquire();
	Events::Subscription& event = m_monitor.accessEvent();

	event.setFd(m_listener.getSockFd());
	event.setMonitoredEvents(Events::Monitor::READ 
							| Events::Monitor::ERROR 
							| Events::Monitor::HANGUP 
							| Events::Monitor::EDGE_TRIGGERED);
	event.setUser(this);
	event.setCallback(EventCallbackAccept);
	m_monitor.subscribe(false);
	return (1);
}

void	ListeningSocket::close()
{
	m_monitor.release();
	m_listener.close();
}

void ListeningSocket::EventCallbackAccept(Events::Subscription& callback)
{
	ListeningSocket*	listener;

	listener = reinterpret_cast<ListeningSocket*>(callback.accessUser());
	listener->accept();
}