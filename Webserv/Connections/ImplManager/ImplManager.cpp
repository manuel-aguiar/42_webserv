

# include "ImplManager.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../../Events/Manager/Manager.hpp"


ImplManager::ImplManager(const size_t maxConnections,
						const std::vector<Ws::BindInfo>& bindAddresses,
						Events::Manager& eventManager, 
						ServerContext& context) :
	m_maxConnections(maxConnections),
	m_connections(m_maxConnections),
	m_spareConnections(m_maxConnections),
	m_listeningSockets(bindAddresses.size()),
	m_pendingAccepts(bindAddresses.size()),
	m_eventManager(eventManager),
	m_context(context)
{

	for (size_t i = 0; i < maxConnections; i++)
	{
		m_connections.emplace_back(*this, m_eventManager, m_context);
		m_connections[i].setEventSubs(m_eventManager.acquireSubscription());
		m_spareConnections.push_back(&m_connections[i]);
	}
}

ImplManager::~ImplManager()
{

}

Conn::Connection* ImplManager::_Listener_ProvideConnection()
{
	InternalConn*     connection;

	if (!m_spareConnections.size())
		return (NULL);
	connection = m_spareConnections.back();
	m_spareConnections.pop_back();
	return (connection);
}



void ImplManager::_Listener_ReturnConnection(Conn::Connection& connection)
{
	InternalConn* internal = static_cast<InternalConn*>(&connection);

	ASSERT_EQUAL(internal >= m_connections.getArray() && internal <= m_connections.getArray() + m_connections.size(), true, 
	"Manager::returnConnection : Connection is not managed by this manager");   //confirm it is in the managed list

	internal->reset();
	m_spareConnections.push_back(internal);
}


//private, as usual

ImplManager::ImplManager(const ImplManager& copy) :
	m_maxConnections(copy.m_maxConnections),
	m_connections(copy.m_connections),
	m_spareConnections(copy.m_spareConnections),
	m_listeningSockets(copy.m_listeningSockets),
	m_pendingAccepts(copy.m_pendingAccepts),
	m_eventManager(copy.m_eventManager),
	m_context(copy.m_context) {}

ImplManager& ImplManager::operator=(const ImplManager& assign)
{
	if (this == &assign) 
		return (*this);

	m_maxConnections = assign.m_maxConnections;
	m_connections = assign.m_connections;
	m_spareConnections = assign.m_spareConnections;
	m_listeningSockets = assign.m_listeningSockets;
	m_pendingAccepts = assign.m_pendingAccepts;

	return (*this);
}