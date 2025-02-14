
# include "InternalManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../InternalListener/InternalListener.hpp"

bool	InternalManager::init()
{
	for (size_t i = 0; i < m_listeningConnInfos.size(); ++i)
	{
		if (!m_listeningConnInfos[i].open())
		{	
			this->shutdown();
			return (false);
		}
	}
	return (true);
}

void   	InternalManager::shutdown()
{
	for (size_t i = 0; i< m_listeningConnInfos.size(); ++i)
		m_listeningConnInfos[i].close();

	m_pendingAccepts.clear();

	for (size_t i = 0; i < m_connections.size(); ++i)
		m_connections[i].forcedClose();
}