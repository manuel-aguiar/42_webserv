/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorker.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/18 08:52:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Project headers
# include "ServerWorker.hpp"
# include "../ServerManager.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Globals/SignalHandler/SignalHandler.hpp"
# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../GenericUtils/Validation/Validation.hpp"

// C++ headers
# include <cstdlib>

ServerWorker::ServerWorker(ServerManager& manager, size_t serverID, Nginx_MemoryPool& pool, Globals& globals) :
	m_myID				(serverID),
	m_serverManager		(manager),
	m_config			(m_serverManager.getConfig()),
	m_connManager		(::atoi(m_serverManager.getConfig().getMaxConnections().c_str()), pool, globals),
	m_eventManager		(globals),
	m_memPool			(pool),
	m_listeners			(m_serverManager.getListenerCount(), Nginx_PoolAllocator<ListeningSocket *>(&m_memPool)),
	m_pendingAccept		(Nginx_MPool_FixedElem<ListeningSocket *>(&m_memPool, m_serverManager.getListenerCount())),
	m_isRunning			(false),
	m_globals			(globals)
{

}

/*
	ListeningSockets are created inside the serverWorker's memory pool which doesn't call destructors
	so we must call it explicitely ourselves. ConnectionManager is the same since it builts its structs
	inside the same memorypool, so we must call its destructor explicitely.

	The alternative to all of this could be to wrap the memory pool inside a shared pointer.
*/
ServerWorker::~ServerWorker()
{
	for (size_t i = 0; i < m_listeners.size(); ++i)
		m_listeners[i]->~ListeningSocket();
	m_listeners.clear();	
	m_pendingAccept.clear();
	m_connManager.~ConnectionManager();
	m_memPool.destroy();
}

int ServerWorker::prepareLaunch()
{
	//setup signal handler
	m_mySignalEvent.setFd_Data_Handler_Flags(g_SignalHandler.getPipeRead(m_myID), this, &ServerWorker::EventExit, EPOLLIN);
	m_eventManager.addEvent(m_mySignalEvent);

	// open listening sockets and monitor them
	for (size_t i = 0; i < m_listeners.size(); ++i)
	{
		if(!m_listeners[i]->open())
			return (0);
		m_eventManager.addEvent(m_listeners[i]->getEvent());
	}
	return (1);
}

int ServerWorker::run()
{
	// run the server
	m_isRunning = true;
	while (m_isRunning)
	{
		// add a mechanism for figuring the right timeout to call
		m_eventManager.retrieveEvents(-1);
	}
	return (1);
}

void	ServerWorker::addPendingAccept(ListeningSocket& listener)
{
	m_pendingAccept.emplace_back(&listener);
}



/*
	Goes to the connection manager, removes the need from listening sockets to access the ConnectionManager
	and instead they interact with their worker to provide it to them, reduces interdependency slightly
*/
Connection*						ServerWorker::provideConnection()
{
	return (m_connManager.provideConnection());
}

/*
	When a Connection is returned to the ConnectionManager, check if there are listeningsockets
	waiting for a free Connection instance such that they can accept.

	If so, try to accept the first on the queue. If successful, move the ListeningSocket to the end of the
	queue (they may have more connections to accept on their backlog, we can't know for certain)

	If accept fails, means the listeningsocket has no backlog to accept so we can remove the pendingAccept

	We do this until either the Connection isntance is reused or we come to the conclusion that there
	is nothing pending right now.
*/
void							ServerWorker::returnConnection(Connection& connection)
{
	m_connManager.returnConnection(connection);
		
	while (m_pendingAccept.size())
	{
		if (m_pendingAccept.front()->accept())
		{
			m_pendingAccept.splice(m_pendingAccept.end(), m_pendingAccept, m_pendingAccept.begin());
			break ;
		}
		m_pendingAccept.pop_front();
	}	
}

ServerWorker::ServerWorker(const ServerWorker& copy) :
	m_myID				(copy.m_myID),
	m_serverManager		(copy.m_serverManager),
	m_config			(m_serverManager.getConfig()),
	m_connManager		(::atoi(m_serverManager.getConfig().getMaxConnections().c_str()), copy.m_memPool, copy.m_globals),
	m_eventManager		(copy.m_globals),
	m_memPool			(copy.m_memPool),
	m_listeners			(Nginx_PoolAllocator<ListeningSocket *>(&m_memPool)),
	m_pendingAccept		(Nginx_MPool_FixedElem<ListeningSocket *>(&m_memPool, m_serverManager.getListenerCount())),
	m_isRunning			(false),
	m_globals			(copy.m_globals)
{(void)copy;}

ServerWorker& ServerWorker::operator=(const ServerWorker& assign) { (void)assign; return (*this);}


void	ServerWorker::createListeningSocket(const t_addrinfo& addrinfo, 
											const int backlog, 
											const t_ptr_ProtoModule& protoModule, 
											const t_func_initProtoConn& initProtoConnection)
{
	ListeningSocket*	newListener;

	newListener = (ListeningSocket*)m_memPool.allocate(sizeof(ListeningSocket));
	new (newListener) ListeningSocket(*this, m_memPool, addrinfo, backlog, m_globals);
	newListener->setProtoModule(protoModule);
	newListener->setInitProtocolConnection(initProtoConnection);
	m_listeners.emplace_back(newListener);
}