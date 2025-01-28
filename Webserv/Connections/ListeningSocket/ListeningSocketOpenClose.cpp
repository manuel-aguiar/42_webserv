

# include "ListeningSocket.hpp"
# include "../ImplManager/ImplManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Events/Manager/Manager.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"


int		ListeningSocket::open()
{
	int res = m_listener.open(m_socket);
	
	if (!res)
		// failed to open
		return (0);

	m_monitor.acquire();
	Events::Subscription* event = m_monitor.accessEvent();

	event->setFd(m_socket.getSockFd());
	event->setMonitoredEvents(Events::Monitor::READ 
							| Events::Monitor::ERROR 
							| Events::Monitor::HANGUP 
							| Events::Monitor::EDGE_TRIGGERED);
	event->setUser(this);
	event->setCallback(EventCallbackAccept);
	m_monitor.subscribe(true);

	return (1);
}

void	ListeningSocket::close()
{
	m_monitor.reset(false);
	m_monitor.release();
	if (m_socket.getSockFd() != -1)
		m_listener.close(m_socket);
}

void ListeningSocket::EventCallbackAccept(Events::Subscription& callback)
{
	ListeningSocket*	listener;

	listener = reinterpret_cast<ListeningSocket*>(callback.accessUser());
	listener->accept();
}