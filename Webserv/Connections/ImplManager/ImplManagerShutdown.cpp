
# include "ImplManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../Events/Manager/Manager.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"

void   	ImplManager::shutdown()
{
	// shutdown listening sockets, no more accepts
	for (size_t i = 0; i< m_listeningSockets.size(); ++i)
	{
		m_listeningSockets[i].close();
		m_eventManager.returnSubscription(*m_listeningSockets[i].accessEvent());
	}

	// here maybe clear the listener queue of listeners that couldn't get a free connection
	m_pendingAccepts.clear();

	// force close all connections, lets the user know we are shutting down, can do own cleanup
	for (size_t i = 0; i < m_connections.size(); ++i)
	{
		m_connections[i].ForceClose();
		m_eventManager.returnSubscription(*m_connections[i].accessEventSubs());
		m_connections[i].setEventSubs(NULL);
	}
}