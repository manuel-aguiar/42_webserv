

# include "ConnectionManager.hpp"
# include "../../EventCallback/EventCallback.hpp"

ConnectionManager::~ConnectionManager()
{

}

ConnectionManager::ConnectionManager(size_t maxConnections, Nginx_MemoryPool& borrowedPool, Globals& globals) :
	m_maxConnections(maxConnections),
	m_connections(m_maxConnections, Nginx_PoolAllocator<ManagedConnection>(borrowedPool)),
	m_readEvents(m_maxConnections, Nginx_PoolAllocator<EventCallback>(borrowedPool)),
	m_writeEvents(m_maxConnections, Nginx_PoolAllocator<EventCallback>(borrowedPool)),
	m_spareConnections(m_maxConnections, Nginx_PoolAllocator<ManagedConnection*>(borrowedPool)),
	m_globals(globals)
{

	for (size_t i = 0; i < maxConnections; i++)
	{
		m_connections.emplace_back(m_globals);
		m_readEvents.emplace_back();
		m_writeEvents.emplace_back();

		m_connections[i].setReadEvent(m_readEvents[i]);
		m_connections[i].setWriteEvent(m_writeEvents[i]);

		m_spareConnections.push_back(&m_connections[i]);
	}
}

Connection* ConnectionManager::provideConnection()
{
	Connection*     connection;

	if (!m_spareConnections.size())
		return (NULL);
	connection = m_spareConnections.back();
	m_spareConnections.pop_back();
	return (connection);
}



void ConnectionManager::returnConnection(Connection& connection)
{
	// Debug
	#ifndef NDEBUG
		CUSTOM_ASSERT(&connection >= &m_connections[0] && &connection <= &m_connections[m_maxConnections],
			"ConnectionManager::returnConnection : Connection is not managed by this manager");   //confirm it is in the managed list
		
		bool test = true;
		for (HeapArray<ManagedConnection*, Nginx_PoolAllocator<ManagedConnection*> >::iterator iter = m_spareConnections.begin(); 
			iter != m_spareConnections.end(); 
			++iter)
		{
			if (*iter == &connection)
			{
				test = false;
				break;
			}
		}
		CUSTOM_ASSERT(test, "ConnectionManager::returnConnection : Connection is already in the spare list");   //confirm it is not in the spare list
	#endif

	connection.reset();
	m_spareConnections.push_back(static_cast<ManagedConnection*>(&connection));

}


//private, as usual

ConnectionManager::ConnectionManager(const ConnectionManager& copy) :
	m_maxConnections(copy.m_maxConnections),
	m_connections(copy.m_connections),
	m_readEvents(copy.m_readEvents),
	m_writeEvents(copy.m_writeEvents),
	m_spareConnections(copy.m_spareConnections),
	m_globals(copy.m_globals)
{(void)copy;}
ConnectionManager& ConnectionManager::operator=(const ConnectionManager& assign) {(void)assign; return (*this);}
