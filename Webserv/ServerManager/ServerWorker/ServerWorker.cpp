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


ServerWorker::ServerWorker(ServerManager& manager, size_t serverID, Globals* globals) :
	m_myID(serverID),
	m_serverManager(manager),
	m_config(m_serverManager.getConfig()),
	m_connManager(m_serverManager.getConfig().getMaxConnections(), globals),						//number of conenctions should come from config
	m_eventManager(globals),
	m_memPool(Nginx_MemoryPool::create(4096, 1)),
	m_isRunning(false),
	m_globals(globals)
{

}

ServerWorker::~ServerWorker()
{
	m_memPool->destroy();
}

int ServerWorker::createListeners(const char* node, const char* port, int socktype, int addrFamily, int backlog)
{
	ListeningSocket     *listener;
	t_addrinfo          hints;
	t_addrinfo          *res;
	t_addrinfo          *cur;
	t_sockaddr*         addr;

	hints = (struct addrinfo){};
	hints.ai_family = addrFamily;
	hints.ai_socktype = socktype;

	int status = getaddrinfo(node, port, &hints, &res);

	if (status != 0)
	{
		m_globals->logStatus("getaddrinfo(): " + std::string(gai_strerror(status)));
		return (1);
	}

	for(cur = res; cur != NULL; cur = cur->ai_next)
	{
		listener = (ListeningSocket *)m_memPool->allocate(sizeof(ListeningSocket), true);
		new (listener) ListeningSocket(*this, m_globals);

		addr = (t_sockaddr *)m_memPool->allocate(cur->ai_addrlen, true);
		std::memcpy(addr, cur->ai_addr, cur->ai_addrlen);
		
		listener->setAddr(addr);
		listener->setSockType(cur->ai_socktype);
		listener->setProtocol(cur->ai_protocol);
		listener->setAddrlen(cur->ai_addrlen);
		listener->setBacklog(backlog);
		listener->setPort(::ntohs(((struct sockaddr_in *)cur->ai_addr)->sin_port));
		listener->accessEvent().setHandlerFunction_and_Data(&ListeningSocket::EventAccept, listener);
		listener->accessEvent().setFlags(EPOLLIN);


		if (listener->open())
			m_listeners.emplace_back(*listener);
		else
		{
			listener->~ListeningSocket();
			continue ;
		}
		//std::cout << "added listener: " << listener->m_sockfd << "and event fd " << listener->m_myEvent.m_fd << std::endl;
		m_eventManager.addEvent(listener->getSocket(), listener->getEvent());

	}
	freeaddrinfo(res);
	return (0);
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
	m_connManager(0, NULL)
{(void)copy;}

ServerWorker& ServerWorker::operator=(const ServerWorker& assign) { (void)assign; return (*this);}
