/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/11 17:11:43 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"
#include "../Connection/ConnectionPool.hpp"
#include "../Globals/Globals.hpp"
#include "../FileDescriptor/FileDescriptor.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Event/HandlerFunction.hpp"


ListeningSocket::ListeningSocket(ConnectionPool& connPool, EventManager& eventManager, Globals* globals) :
    m_globals(globals),
    m_connectionPool(connPool),
    m_eventManager(eventManager)
{
    #if !defined(NDEBUG) && defined(DEBUG_CTOR)
        #include <iostream>
        if (globals)
            m_globals->m_debugFile->record("ListeningSocket Constructor Called");
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
        m_globals->m_debugFile->record("ListeningSocket Destructor Called");
    else
        std::cout << "ListeningSocket Destructor Called" << std::endl;
#endif

}

int    ListeningSocket::open()
{
    int options;

    m_sockfd = ::socket(m_addr->sa_family, m_socktype, m_proto);
    //std::cout << "listener sockfd " << m_sockfd << std::endl;
    if (m_sockfd == -1)
    {
        m_globals->logStatus("socket(): " + std::string(strerror(errno)));
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

    if (::setsockopt(m_sockfd, SOL_SOCKET, options, &options, sizeof(options)) == -1)
    {
        m_globals->logStatus("setsockopt(): " + std::string(strerror(errno)));
        return (0);
    }

    if (!bind() || !listen())
        return (0);
    return (1);
}

int    ListeningSocket::bind()
{
    if (::bind(m_sockfd, m_addr, m_addrlen) == -1)
    {
        m_globals->logStatus("bind(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1);
}

int    ListeningSocket::listen()
{
    if (::listen(m_sockfd, m_backlog) == -1)
    {
        m_globals->logStatus("listen(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1);
}

void    ListeningSocket::accept()
{
    Connection* connection;
    u_sockaddr  addr;
    t_socklen   addrlen;


    connection = m_connectionPool.getConnection();

    if (!connection)
    {
        std::cout << "       connection pool empty" << std::endl;
        return ;
    }

    connection->m_listener = this;
    addrlen = sizeof(addr);
    connection->m_sockfd = ::accept(m_sockfd, &addr.sockaddr, &addrlen);

    if (connection->m_sockfd == -1)
        goto NewConnection_Failure;

    if (!FileDescriptor::setCloseOnExec_NonBlocking(connection->m_sockfd))
        goto NewConnection_Failure;
    connection->m_readEvent->m_fd = connection->m_sockfd;
    connection->m_writeEvent->m_fd = connection->m_sockfd;
    connection->m_addr = (t_sockaddr*)connection->m_memPool->allocate(addrlen, true);

    if (!connection->m_addr)
        goto NewConnection_Failure;

    std::memcpy(connection->m_addr, &addr, addrlen);
    connection->m_addrlen = addrlen;
 //std::cout << "added conenction" << std::endl;
    if (!m_eventManager.addEvent(connection->m_sockfd, *connection->m_readEvent))
        goto NewConnection_Failure;



    return ;

NewConnection_Failure:
    m_globals->logStatus("ListeningSocket::listener_Accept(): " + std::string(strerror(errno)));
    _close_accepted_connection(connection);

}

void    ListeningSocket::_close_accepted_connection(Connection* connection)
{
    if (connection->m_sockfd != -1 && ::close(connection->m_sockfd) == -1)
        m_globals->logStatus("close(): " + std::string(strerror(errno)));
    connection->m_sockfd = -1;
    connection->reset();
    m_connectionPool.returnConnection(connection);
}

void    ListeningSocket::closeConnection(Connection* connection)
{
    m_eventManager.delEvent(connection->m_sockfd);
    _close_accepted_connection(connection);

}


void    ListeningSocket::close()
{
    if (::close(m_sockfd) == -1)
        m_globals->logStatus("close(): " + std::string(strerror(errno)));
}



//private
ListeningSocket::ListeningSocket() :
    m_globals(NULL),
    m_connectionPool(*((ConnectionPool*)NULL)),  //never do this, for real
    m_eventManager(*((EventManager*)NULL))       //never do this, for real
{

#if !defined(NDEBUG) && defined(DEBUG_CTOR)
    #include <iostream>
    if (globals)
        m_globals->m_debugFile->record("ListeningSocket Destructor Called");
    else
        std::cout << "ListeningSocket Destructor Called" << std::endl;
#endif

}
