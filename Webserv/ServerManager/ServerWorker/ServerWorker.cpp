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

# include "ServerWorker.hpp"
# include "../ServerManager.hpp"
# include "../../Globals/Globals.hpp"
# include "../../Globals/SignalHandler/SignalHandler.hpp"
# include "../../ServerConfig/ServerConfig.hpp"


ServerWorker::ServerWorker(ServerManager& manager, size_t serverID, Nginx_MemoryPool* pool, Globals* globals) :
	m_myID				(serverID),
	m_serverManager		(manager),
	m_config			(m_serverManager.getConfig()),
	m_connManager		(m_serverManager.getConfig().getMaxConnections(), pool, globals),
	m_eventManager		(globals),
	m_memPool			(pool),
	m_listeners			(Nginx_PoolAllocator<ListeningSocket *>(m_memPool)),
	m_pendingAccept		(Nginx_MPool_FixedElem<ListeningSocket *>(m_memPool, m_serverManager.getListenerCount())),
	m_isRunning			(false),
	m_globals			(globals)
{

}

ServerWorker::~ServerWorker()
{
	m_memPool->destroy();
}


int ServerWorker::run()
{

	//setup signal handler
	m_mySignalEvent.setFd_Data_Handler_Flags(SignalHandler::PipeRead(m_myID), this, &ServerWorker::EventExit, EPOLLIN);
	m_eventManager.addEvent(m_mySignalEvent);

	// open listening sockets and monitor them
	for (size_t i = 0; i < m_listeners.size(); ++i)
	{
		if(!m_listeners[i]->open())
			return (1);
		m_eventManager.addEvent(m_listeners[i]->getEvent());
	}

	// run the server
	m_isRunning = true;
	while (m_isRunning)
	{
		m_eventManager.waitEvents(-1);
		m_eventManager.distributeEvents();
	}
	return (1);
}

/*
	Ok, accept fails, add to pending, should unsubscribe from epoll...?
	in reality should unsubscribe all listening sockets from epoll until there are connections available
	to fullfill the pending accepts. Otherwise the list of pending could grow infinite for no reason
	because the same listener could be there a load of times before there is one that becomes free.

	when a listener returns a connection to the pool, that should trigger a check if there are pending listeners
	waiting for connections, to get one and accept it.

	However, if i unsubscribe from epoll, i will essentially miss out on all the potential accept monitors because
	epoll will not be monitoring while they are down. It could trigger 1 accept but there were 10 missed.

	I can however loop read the socket until i get an error and then i will know that there are no more connections.

	Maybe listening sockets should subscribe EPOLLET... food for thought
*/

void	ServerWorker::addPendingAccept(ListeningSocket* listener)
{
	m_pendingAccept.push_back(listener);
}

ServerWorker::ServerWorker(const ServerWorker& copy) :
	m_myID				(copy.m_myID),
	m_serverManager		(copy.m_serverManager),
	m_config			(m_serverManager.getConfig()),
	m_connManager		(m_serverManager.getConfig().getMaxConnections(), copy.m_memPool, copy.m_globals),
	m_eventManager		(copy.m_globals),
	m_memPool			(copy.m_memPool),
	m_listeners			(Nginx_PoolAllocator<ListeningSocket *>(m_memPool)),
	m_pendingAccept		(Nginx_MPool_FixedElem<ListeningSocket *>(m_memPool, m_serverManager.getListenerCount())),
	m_isRunning			(false),
	m_globals			(copy.m_globals)
{(void)copy;}

ServerWorker& ServerWorker::operator=(const ServerWorker& assign) { (void)assign; return (*this);}
