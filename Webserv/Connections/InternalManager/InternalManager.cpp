

# include "InternalManager.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"
# include "../InternalConn/InternalConn.hpp"
# include "../../Events/Manager/Manager.hpp"






// erase, for tests only
# include "../../Events/Subscription/Subscription.hpp"

InternalManager::InternalManager(const size_t maxConnections,
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
		m_listeningSockets.emplace_back(bindAddresses[i], *this);
	}
}

InternalManager::~InternalManager()
{
	shutdown();
}

InternalConn* InternalManager::_Accepter_ProvideConnection()
{
	InternalConn*     connection;

	if (!m_spareConnections.size())
		return (NULL);
	connection = m_spareConnections.back();
	m_spareConnections.pop_back();
	return (connection);
}

void InternalManager::_Accepter_MoveToPendingAccept(ListeningSocket& listener)
{
	m_pendingAccepts.push_back(&listener);
}

void InternalManager::_ReturnConnection(Conn::Connection& connection)
{
	InternalConn* 	internal = static_cast<InternalConn*>(&connection);

	ASSERT_EQUAL(internal >= m_connections.getArray() && internal <= m_connections.getArray() + m_connections.size(), true, 
	"Manager::returnConnection : Connection is not managed by this manager");   //confirm it is in the managed list

	internal->reset();
	//if (internal->accessEvent().isSubscribed())
	//	std::cout <<  internal << " InternalManager::_ReturnConnection() : Connection event still subscribed after reset" << std::endl;
	//else
	//	std::cout <<  internal << " InternalManager::_ReturnConnection() : Connection event is not subscribed after reset" << std::endl;
	while (m_pendingAccepts.size())
	{
		ListeningSocket* listener = m_pendingAccepts.front();
		m_pendingAccepts.pop_front();
		//std::cout << internal << " before trying listener, is event subscribed? " << internal->accessEvent().isSubscribed() << std::endl;
		if (listener->acceptPending(*internal) != -1) // success, push it back again, may have more to accept
		{
			//std::cout <<  internal << " success this listener, is event subscribed? " << internal->accessEvent().isSubscribed() << std::endl;
			m_pendingAccepts.push_back(listener);
			return ;
		}
		//else
		//{
		//	std::cout << internal << " trying another listener, is event subscribed? " << internal->accessEvent().isSubscribed() << std::endl;
		//}
		// else, this socket has no more to accept, move on to the next
	}
	//nobody to take the connection back in circulation, push it back in the spare list
	m_spareConnections.push_back(internal);
}


Events::Manager& InternalManager::_accessEventManager()
{
	return (m_eventManager);
}

Globals& InternalManager::_accessGlobals()
{
	return (m_globals);
}

ServerContext& InternalManager::_accessServerContext()
{
	return (m_context);
}

//private, as usual

InternalManager::InternalManager(const InternalManager& copy) :
	m_eventManager(copy.m_eventManager),
	m_globals(copy.m_globals),
	m_context(copy.m_context) {}

InternalManager& InternalManager::operator=(const InternalManager& assign)
{
	if (this == &assign) 
		return (*this);

	return (*this);
}