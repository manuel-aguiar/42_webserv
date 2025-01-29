
# include "InternalManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"

bool	InternalManager::init()
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

void   	InternalManager::shutdown()
{
	for (size_t i = 0; i< m_listeningSockets.size(); ++i)
		m_listeningSockets[i].close();

	m_pendingAccepts.clear();

	for (size_t i = 0; i < m_connections.size(); ++i)
		m_connections[i].ForcedClose();
}