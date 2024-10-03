/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 13:40:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "../Globals/Globals.hpp"

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
    _listener = NULL;
    _memPool->reset();
}

void    Connection::read()
{
    _globals->_logFile->record("RUUUUUAD");

    char buf[1024];
    while (1)
    {
        int bytes = recv(_sockfd, buf, 1024, 0);
        if (bytes == -1)
        {
            _globals->_logFile->record("recv(): " + std::string(std::strerror(errno)));
            return ;
        }
        if (bytes == 0)
        {
            _globals->_logFile->record("Connection closed by client");
            return ;
        }
        buf[bytes] = '\0';
        _globals->_logFile->record("Received: " + std::string(buf));
    }   
}

void    Connection::write() {}



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
