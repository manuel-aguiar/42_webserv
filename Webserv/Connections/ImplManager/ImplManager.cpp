

# include "ImplManager.hpp"


ImplManager::ImplManager(size_t maxConnections, Events::Manager& eventManager, Globals& globals) :
	m_maxConnections(maxConnections),
	m_connections(m_maxConnections),
	m_spareConnections(m_maxConnections),
	m_eventManager(eventManager),
	m_globals(globals)
{

	for (size_t i = 0; i < maxConnections; i++)
	{
		m_connections.emplace_back(m_globals);

		m_connections[i].setReadEvent(m_readEvents[i]);
		m_connections[i].setWriteEvent(m_writeEvents[i]);

		m_spareConnections.push_back(&m_connections[i]);
	}
}

ImplManager::~ImplManager()
{

}

Connection* ImplManager::provideConnection()
{
	Connection*     connection;

	if (!m_spareConnections.size())
		return (NULL);
	connection = m_spareConnections.back();
	m_spareConnections.pop_back();
	return (connection);
}



void ImplManager::returnConnection(Connection& connection)
{
	// Debug
	#ifndef NDEBUG
		CUSTOM_ASSERT(&connection >= &m_connections[0] && &connection <= &m_connections[m_maxConnections],
			"Manager::returnConnection : Connection is not managed by this manager");   //confirm it is in the managed list
		
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
		CUSTOM_ASSERT(test, "Manager::returnConnection : Connection is already in the spare list");   //confirm it is not in the spare list
	#endif

	connection.reset();
	m_spareConnections.push_back(static_cast<ManagedConnection*>(&connection));
}


//private, as usual

ImplManager::ImplManager(const ImplManager& copy) :
	m_maxConnections(copy.m_maxConnections),
	m_connections(copy.m_connections),
	m_readEvents(copy.m_readEvents),
	m_writeEvents(copy.m_writeEvents),
	m_spareConnections(copy.m_spareConnections),
	m_globals(copy.m_globals)
{(void)copy;}
ImplManager& ImplManager::operator=(const ImplManager& assign) {(void)assign; return (*this);}