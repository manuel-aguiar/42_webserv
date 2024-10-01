/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 19:25:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"
#include "../Connection/ConnectionPool.hpp"

ListeningSocket::ListeningSocket(ConnectionPool& connPool, ILog* logFile) :
    _logFile(logFile),
    _connectionPool(connPool)
{

}

ListeningSocket::~ListeningSocket()
{
    _connectionPool.returnConnection(_myConnection);
    close();
}

int    ListeningSocket::open()
{
    int options;

    _sockfd = ::socket(_addr->sa_family, _socktype, _proto);
    
    if (_sockfd == -1)
    {
        _logFile->record("socket(): " + std::string(strerror(errno)));
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
        _logFile->record("setsockopt(): " + std::string(strerror(errno)));
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
        _logFile->record("bind(): " + std::string(strerror(errno)));
        return (0);
    }
    return (1);
}

int    ListeningSocket::listen()
{
    if (::listen(_sockfd, _backlog) == -1)
    {
        _logFile->record("listen(): " + std::string(strerror(errno)));
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
        _logFile->record("ConnectionPool exhausted");
        return ;
    }
    connection->_listener = this;
    addrlen = sizeof(addr);
    connection->_sockfd = ::accept(_sockfd, &addr.sockaddr, &addrlen);
    
    if (connection->_sockfd == -1)
    {
        _logFile->record("accept(): " + std::string(strerror(errno)));
        throw std::runtime_error("accept() failed");        //meh, later
    }


    /*
        TODO: set nonblocking
    */

    connection->_addr = (t_sockaddr*)connection->_connectionAlloc->allocate(addrlen, true);
    std::memcpy(connection->_addr, &addr, addrlen);
    connection->_addrlen = addrlen;
}

void    ListeningSocket::close()
{
    if (::close(_sockfd) == -1)
        _logFile->record("close(): " + std::string(strerror(errno)));
}

