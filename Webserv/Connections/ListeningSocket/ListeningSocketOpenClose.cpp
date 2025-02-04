

# include "ListeningSocket.hpp"
# include "../../Events/Subscription/Subscription.hpp"

# include <iostream>
int		ListeningSocket::open()
{
	int res = m_accepter.open(m_socket);
	
	if (!res)
		// failed to open
		return (0);

	m_monitor.acquire(mf_accessEventManager());
	Events::Subscription& event = m_monitor.accessEvent();

	event.setFd(m_socket.getSockFd());
	event.setMonitoredEvents(Events::Monitor::READ 
							| Events::Monitor::ERROR 
							| Events::Monitor::HANGUP 
							| Events::Monitor::EDGE_TRIGGERED);
	event.setUser(this);
	event.setCallback(EventCallbackAccept);
	m_monitor.subscribe(mf_accessEventManager(), false);
	return (1);
}

void	ListeningSocket::close()
{
	m_monitor.reset(mf_accessEventManager(), false);
	m_monitor.release(mf_accessEventManager());
	if (m_socket.getSockFd() != -1)
		m_accepter.close(m_socket);
}

void ListeningSocket::EventCallbackAccept(Events::Subscription& callback)
{
	ListeningSocket*	listener;

	listener = reinterpret_cast<ListeningSocket*>(callback.accessUser());
	listener->accept();
}