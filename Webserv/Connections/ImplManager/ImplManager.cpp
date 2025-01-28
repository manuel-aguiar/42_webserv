

# include "ImplManager.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../../Events/Manager/Manager.hpp"


ImplManager::ImplManager(const size_t maxConnections,
						const std::vector<Ws::BindInfo>& bindAddresses,
						Events::Manager& eventManager, 
						Globals& globals,
						ServerContext& context) :
	m_maxConnections(maxConnections),
	m_connections(m_maxConnections),
	m_spareConnections(m_maxConnections),
	m_listeningSockets(bindAddresses.size()),
	m_pendingAccepts(bindAddresses.size()),
	m_eventManager(eventManager),
	m_globals(globals),
	m_context(context)
{

	for (size_t i = 0; i < maxConnections; i++)
	{
		m_connections.emplace_back(*this);
		m_spareConnections.push_back(&m_connections[i]);
	}
	for (size_t i = 0; i < bindAddresses.size(); i++)
	{
		m_listeningSockets.emplace_back(100, bindAddresses[i], *this);
	}
}

ImplManager::~ImplManager()
{
	shutdown();
}

InternalConn* ImplManager::_Listener_ProvideConnection()
{
	InternalConn*     connection;

	if (!m_spareConnections.size())
		return (NULL);
	connection = m_spareConnections.back();
	m_spareConnections.pop_back();
	return (connection);
}

void ImplManager::_Listener_MoveToPendingAccept(ListeningSocket& listener)
{
	m_pendingAccepts.push_back(&listener);
}

void ImplManager::_ReturnConnection(Conn::Connection& connection)
{
	InternalConn* 	internal = static_cast<InternalConn*>(&connection);

	ASSERT_EQUAL(internal >= m_connections.getArray() && internal <= m_connections.getArray() + m_connections.size(), true, 
	"Manager::returnConnection : Connection is not managed by this manager");   //confirm it is in the managed list

	internal->reset();
	while (m_pendingAccepts.size())
	{
		ListeningSocket* listener = m_pendingAccepts.front();
		m_pendingAccepts.pop_front();
		if (listener->acceptPending(*internal) != -1) // success, push it back again, may have more to accept
		{
			m_pendingAccepts.push_back(listener);
			return ;
		};
		// else, this socket has no more to accept, move on to the next
	}
	//nobody to take the connection back in circulation, push it back in the spare list
	m_spareConnections.push_back(internal);
}


Events::Manager& ImplManager::_accessEventManager()
{
	return (m_eventManager);
}

Globals& ImplManager::_accessGlobals()
{
	return (m_globals);
}

ServerContext& ImplManager::_accessServerContext()
{
	return (m_context);
}

//private, as usual

ImplManager::ImplManager(const ImplManager& copy) :
	m_eventManager(copy.m_eventManager),
	m_globals(copy.m_globals),
	m_context(copy.m_context) {}

ImplManager& ImplManager::operator=(const ImplManager& assign)
{
	if (this == &assign) 
		return (*this);

	return (*this);
}