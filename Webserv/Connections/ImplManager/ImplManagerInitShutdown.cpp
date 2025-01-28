
# include "ImplManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"

bool	ImplManager::init()
{
	for (size_t i = 0; i < m_listeningSockets.size(); ++i)
	{
		if (!m_listeningSockets[i].open())
		{	
			this->shutdown();
			return (false);
		}
	}
	return (true);
}

void   	ImplManager::shutdown()
{
	for (size_t i = 0; i< m_listeningSockets.size(); ++i)
		m_listeningSockets[i].close();

	m_pendingAccepts.clear();

	for (size_t i = 0; i < m_connections.size(); ++i)
		m_connections[i].ForcedClose();
}