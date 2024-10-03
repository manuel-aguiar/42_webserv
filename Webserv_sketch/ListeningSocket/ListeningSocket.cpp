/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 16:36:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"
#include "../Connection/ConnectionPool.hpp"
#include "../Globals/Globals.hpp"
#include "../FileDescriptor/FileDescriptor.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Event/HandlerFunction.hpp"


ListeningSocket::ListeningSocket(ConnectionPool& connPool, EventManager& eventManager, Globals* globals) :
    _globals(globals),
    _connectionPool(connPool),
    _eventManager(eventManager)
{
    #if !defined(NDEBUG) && defined(DEBUG_CTOR)
        #include <iostream>
        if (globals)
            _globals->_debugFile->record("ListeningSocket Constructor Called");
        else
            std::cout << "ListeningSocket Constructor Called" << std::endl;
    #endif
}

ListeningSocket::~ListeningSocket()
{
    close();

#if !defined(NDEBUG) && defined(DEBUG_CTOR)
    #include <iostream>
    if (globals)
        _globals->_debugFile->record("ListeningSocket Destructor Called");
    else
        std::cout << "ListeningSocket Destructor Called" << std::endl;
#endif

}

int    ListeningSocket::open()
{
    int options;

    _sockfd = ::socket(_addr->sa_family, _socktype, _proto);
    
    if (_sockfd == -1)
    {
        _globals->_logFile->record("socket(): " + std::string(strerror(errno)));
        return (0);
    }

    /*
        TODO: fcntl set nonblocking
    */
    
    #ifdef SO_REUSEPORT
        options = SO_REUSEADDR | SO_REUSEPORT;
    #else
        options = SO_REUSEADDR;
    #endif

    if (::setsockopt(_sockfd, SOL_SOCKET, options, &options, sizeof(options)) == -1)
    {
        _globals->_logFile->record("setsockopt(): " + std::string(strerror(errno)));
        return (0);
    }   

    if (!bind() || !listen())
        return (0);
    return (1);
}

int    ListeningSocket::bind()
{
    if (::bind(_sockfd, _addr, _addrlen) == -1)
    {
        _globals->_logFile->record("bind(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1);
}

int    ListeningSocket::listen()
{
    if (::listen(_sockfd, _backlog) == -1)
    {
        _globals->_logFile->record("listen(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1);
}

void    ListeningSocket::accept()
{
    Connection* connection;
    u_sockaddr  addr;
    t_socklen   addrlen;

    
    connection = _connectionPool.getConnection();
    
    if (!connection)
    {
        //_globals->_logFile->record("ConnectionPool exhausted");
        return ;
    }

    connection->_listener = this;
    addrlen = sizeof(addr);
    connection->_sockfd = ::accept(_sockfd, &addr.sockaddr, &addrlen);
    
    if (connection->_sockfd == -1)
        goto NewConnection_Failure;

    if (!FileDescriptor::setCloseOnExec_NonBlocking(connection->_sockfd))
        goto NewConnection_Failure;

    connection->_addr = (t_sockaddr*)connection->_memPool->allocate(addrlen, true);
    
    if (!connection->_addr)
        goto NewConnection_Failure;

    std::memcpy(connection->_addr, &addr, addrlen);
    connection->_addrlen = addrlen;

    if (!_eventManager.addEvent(connection->_sockfd, *connection->_readEvent))
        goto NewConnection_Failure;
        
    return ;

NewConnection_Failure:
    _globals->_logFile->record("ListeningSocket::listener_Accept(): " + std::string(strerror(errno)));
    _close_accepted_connection(connection);
    
}

void    ListeningSocket::_close_accepted_connection(Connection* connection)
{
    if (connection->_sockfd != -1 && ::close(connection->_sockfd) == -1)
        _globals->_logFile->record("close(): " + std::string(strerror(errno)));
    connection->_sockfd = -1;
    connection->reset();
    _connectionPool.returnConnection(connection);
}

void    ListeningSocket::closeConnection(Connection* connection)
{
    _eventManager.delEvent(connection->_sockfd);
    _close_accepted_connection(connection);
    
}


void    ListeningSocket::close()
{
    if (::close(_sockfd) == -1)
        _globals->_logFile->record("close(): " + std::string(strerror(errno)));
}



//private
ListeningSocket::ListeningSocket() : 
    _globals(NULL),
    _connectionPool(*((ConnectionPool*)NULL)),  //never do this, for real
    _eventManager(*((EventManager*)NULL))       //never do this, for real
{

#if !defined(NDEBUG) && defined(DEBUG_CTOR)
    #include <iostream>
    if (globals)
        _globals->_debugFile->record("ListeningSocket Destructor Called");
    else
        std::cout << "ListeningSocket Destructor Called" << std::endl;
#endif

}
