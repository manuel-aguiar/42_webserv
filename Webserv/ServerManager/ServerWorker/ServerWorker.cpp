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

# include "ServerManager.hpp"
# include "../Globals/Globals.hpp"

ServerWorker::ServerWorker(size_t serverID, Globals* _globals) :
    m_myID(serverID),
    m_pool(Nginx_MemoryPool::create(4096, 1)),
    m_connectionPool(_globals),
    m_eventManager(_globals),
    m_globals(_globals),
    m_isRunning(true)
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
        new (listener) ListeningSocket(m_connectionPool, m_eventManager, m_globals);

        listener->m_addr = (t_sockaddr *)m_pool->allocate(cur->ai_addrlen, true);
        std::memcpy(listener->m_addr, cur->ai_addr, cur->ai_addrlen);


        listener->m_socktype = cur->ai_socktype;
        listener->m_proto = cur->ai_protocol;
        listener->m_addrlen = cur->ai_addrlen;
        listener->m_backlog = backlog;
        listener->m_myEvent.setHandler_Function_and_Data(&HandlerFunction::listener_Accept, listener);
        listener->m_myEvent.setFlags(EPOLLIN);


        if (listener->open())
            m_listeners.push_back(listener);
        else
        {
            listener->~ListeningSocket();
            continue ;
        }
        listener->m_myEvent.m_fd = listener->m_sockfd;
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
    m_mySignalEvent.setHandler_Function_and_Data(&HandlerFunction::signal_Read, this);
    m_mySignalEvent.setFlags(EPOLLIN);
    m_mySignalEvent.m_fd = pipeRead;
    //std::cout << "added pipoe" << std::endl;
    m_eventManager.addEvent(pipeRead, m_mySignalEvent);

    return (1);
}

int ServerWorker::run()
{
    while (m_isRunning)
    {
        m_eventManager.waitEvents(-1);
        m_eventManager.distributeEvents();
    }
    return (1);
}

//private
ServerWorker::ServerWorker() :
    m_connectionPool(NULL, 0) {}

ServerWorker::ServerWorker(const ServerWorker& copy) :
    m_connectionPool(NULL, 0)  {(void)copy;}

ServerWorker& ServerWorker::operator=(const ServerWorker& assign) { (void)assign; return (*this);}
