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
# include "../Globals/Globals.hpp"


ServerWorker::ServerWorker(ServerManager& manager, size_t serverID, Globals* _globals) :
    m_myID(serverID),
    m_pool(Nginx_MemoryPool::create(4096, 1)),
    m_connManager(_globals, manager.getConfig().get),						//number of conenctions should come from config
    m_eventManager(_globals),
    m_globals(_globals),
    m_isRunning(false),
    m_serverManager(manager)
{
}

ServerWorker::~ServerWorker()
{
    m_pool->destroy();
}

int ServerWorker::createListeners(const char* node, const char* port, int socktype, int addrFamily, int backlog)
{
    ListeningSocket*    listener;
    t_addrinfo          hints;
    t_addrinfo          *res;
    t_addrinfo          *cur;

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
        listener = (ListeningSocket *)m_pool->allocate(sizeof(ListeningSocket), true);
        new (listener) ListeningSocket(*this, m_connManager, m_eventManager, m_globals);

        listener->m_addr = (t_sockaddr *)m_pool->allocate(cur->ai_addrlen, true);
        std::memcpy(listener->m_addr, cur->ai_addr, cur->ai_addrlen);


        listener->m_socktype = cur->ai_socktype;
        listener->m_proto = cur->ai_protocol;
        listener->m_addrlen = cur->ai_addrlen;
        listener->m_backlog = backlog;
        listener->m_myEvent.setHandlerFunction_and_Data(&ListeningSocket::EventAccept, listener);
        listener->m_myEvent.setFlags(EPOLLIN);


        if (listener->open())
            m_listeners.push_back(listener);
        else
        {
            listener->~ListeningSocket();
            continue ;
        }
        //std::cout << "added listener: " << listener->m_sockfd << "and event fd " << listener->m_myEvent.m_fd << std::endl;
        m_eventManager.addEvent(listener->m_sockfd, listener->m_myEvent);

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
    m_connManager(NULL, 0),
    m_serverManager(copy.m_serverManager),
	m_config(copy.m_config)
{(void)copy;}

ServerWorker& ServerWorker::operator=(const ServerWorker& assign) { (void)assign; return (*this);}
