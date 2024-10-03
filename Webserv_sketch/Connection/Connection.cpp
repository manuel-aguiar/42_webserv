/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 16:36:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "../Globals/Globals.hpp"
#include "../ListeningSocket/ListeningSocket.hpp"

Connection::Connection(Globals* globals) :
    _memPool(NULL),
    _globals(globals)
{
    
}

Connection::~Connection()
{

}

void    Connection::init()
{
    _memPool = Nginx_MemoryPool::create(4096, 10);
}

void    Connection::reset()
{
    _sockfd = -1;
    _listener = NULL;
    _memPool->reset();
}

void    Connection::read()
{
    //_globals->_logFile->record("RUUUUUAD");

    char buf[1024];

    int bytes = ::read(_sockfd, buf, 1024);
    if (bytes == -1)
    {
        //_globals->_logFile->record("read(): " + std::string(std::strerror(errno)));
        _listener->closeConnection(this);
        return ;
    }
    if (bytes == 0)
    {
        _listener->closeConnection(this);
        //_globals->_logFile->record("connection gracefully closed by the client");
        return ;
    }
    buf[bytes] = '\0';
    //_globals->_logFile->record("Received: " + std::string(buf));

    write();

}

void    Connection::write() { ::write(_sockfd, "Hello Client!", sizeof("Hello Client!")); }



// no copies, as usual
Connection::Connection(const Connection& other) :
    _memPool(other._memPool),
    _globals(other._globals)
{
    (void)other;
}
Connection& Connection::operator=(const Connection& other)
{
    if (this == &other)
        return (*this);
    _memPool = other._memPool;
    _globals = other._globals;
    return (*this);
}
