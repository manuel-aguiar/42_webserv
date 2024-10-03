/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:03 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 13:33:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"
# include "../Globals/Globals.hpp"

Server::Server(size_t serverID, Globals* _globals) :
    _myID(serverID),
    _pool(Nginx_MemoryPool::create(4096, 1)),
    _connectionPool(_globals),
    _eventManager(_globals),
    _globals(_globals),
    _isRunning(true)
{
    #ifdef SO_REUSEPORT
        _multithreadListen = true;
    #else
        _multithreadListen = false;
    #endif
}

Server::~Server()
{
    _pool->destroy();
}

int Server::createListeners(const char* node, const char* port, int socktype, int addrFamily, int backlog)
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
        _globals->_logFile->record("getaddrinfo(): " + std::string(gai_strerror(status)));
		return (1);
	}
    
    for(cur = res; cur != NULL; cur = cur->ai_next)
	{
        listener = (ListeningSocket *)_pool->allocate(sizeof(ListeningSocket), true);
        new (listener) ListeningSocket(_connectionPool, _eventManager, _globals);
        
        listener->_addr = (t_sockaddr *)_pool->allocate(cur->ai_addrlen, true);
        std::memcpy(listener->_addr, cur->ai_addr, cur->ai_addrlen);


        listener->_socktype = cur->ai_socktype;
        listener->_proto = cur->ai_protocol;
        listener->_addrlen = cur->ai_addrlen;
        listener->_backlog = backlog;
        listener->_myEvent.setHandler_Function_and_Data(&HandlerFunction::listener_Accept, listener);
        listener->_myEvent.setFlags(EPOLLIN);
        
        if (listener->open())
            _listeners.push_back(listener);
        else
        {
            listener->~ListeningSocket();
            continue ;
        }
            
        _eventManager.addEvent(listener->_sockfd, listener->_myEvent);
    }   
    freeaddrinfo(res);
    return (0);
}

int Server::setup_mySignalHandler()
{
    int pipeRead;

    pipeRead = SignalHandler::PipeRead(_myID);
    _mySignalEvent.setHandler_Function_and_Data(&HandlerFunction::signal_Read, this);
    _mySignalEvent.setFlags(EPOLLIN);
    _eventManager.addEvent(pipeRead, _mySignalEvent);
    return (1);
}

int Server::run()
{
    while (_isRunning)
    {
        _eventManager.waitEvents(-1);
        _eventManager.distributeEvents();
    }
    return (1);
}

//private
Server::Server() :
    _connectionPool(NULL, 0) {} 

Server::Server(const Server& copy) :
    _connectionPool(NULL, 0)  {(void)copy;}

Server& Server::operator=(const Server& assign) { (void)assign; return (*this);}