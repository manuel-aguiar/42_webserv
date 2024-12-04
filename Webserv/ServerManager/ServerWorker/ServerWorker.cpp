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

static size_t countListeners(const ServerConfig& config)
{
	typedef std::map<std::string, ServerBlocks> t_blocks;

	size_t count = 0;
	const t_blocks& blocks = config.getServerBlocks();

	for (t_blocks::const_iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
		count += iter->second.getListeners().size();
	
	return (count);
}

ServerWorker::ServerWorker(ServerManager& manager, size_t serverID, Nginx_MemoryPool* pool, Globals* globals) :
	m_myID				(serverID),
	m_serverManager		(manager),
	m_config			(m_serverManager.getConfig()),
	m_connManager		(m_serverManager.getConfig().getMaxConnections(), pool, globals),
	m_eventManager		(globals),
	m_memPool			(pool),
	m_listeners			(Nginx_PoolAllocator<ListeningSocket>(pool)),
	m_isRunning			(false),
	m_globals			(globals)
{

}

ServerWorker::~ServerWorker()
{
	m_memPool->destroy();
}

int ServerWorker::createListeners(const char* node, const char* port, int socktype, int addrFamily, int backlog)
{

}

int ServerWorker::setup_mySignalHandler()
{
	int pipeRead;

	pipeRead = SignalHandler::PipeRead(m_myID);
	m_mySignalEvent.setHandlerFunction_and_Data(&ServerWorker::EventExit, this);
	m_mySignalEvent.setFlags(EPOLLIN);
	m_eventManager.addEvent(pipeRead, m_mySignalEvent);

	return (1);
}

int ServerWorker::run()
{
	m_isRunning = true;
	while (m_isRunning)
	{
		m_eventManager.waitEvents(-1);
		m_eventManager.distributeEvents();
	}
	return (1);
}

ServerWorker::ServerWorker(const ServerWorker& copy) :
	m_serverManager(copy.m_serverManager),
	m_config(copy.m_config),
	m_connManager(0, copy.m_memPool, NULL)
{(void)copy;}

ServerWorker& ServerWorker::operator=(const ServerWorker& assign) { (void)assign; return (*this);}
