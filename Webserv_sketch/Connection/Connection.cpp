/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 19:51:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"


Connection::Connection(ILog* logFile) :
    _connectionAlloc(NULL),
    _logFile(logFile)
{
    _logFile = logFile;
}

Connection::~Connection()
{

}

void    Connection::init()
{
    _connectionAlloc = Nginx_MemoryPool::create(4096, 10);
}

void    Connection::reset()
{
    _listener = NULL;
    _connectionAlloc->reset();
}

void    Connection::read() {}
void    Connection::write() {}

// no copies, as usual
Connection::Connection(const Connection& other) :
    _connectionAlloc(other._connectionAlloc)
{
    (void)other;
}
Connection& Connection::operator=(const Connection& other)
{
    if (this == &other)
        return (*this);
    _connectionAlloc = other._connectionAlloc;
    return (*this);
}
