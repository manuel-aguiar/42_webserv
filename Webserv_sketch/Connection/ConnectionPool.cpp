/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 08:44:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionPool.hpp"


ConnectionPool::~ConnectionPool() 
{

}

ConnectionPool::ConnectionPool(ILog* logFile, size_t maxConnections) : 
    _logFile(logFile), 
    _maxConnections(maxConnections),
    _spareConnMemPool(maxConnections),
    _spareConnections(_spareConnMemPool)
{
    _connections.reserve(maxConnections);
    _readEvents.reserve(maxConnections);
    _writeEvents.reserve(maxConnections);

    for (size_t i = 0; i < maxConnections; i++)
    {
        _connections.push_back(Connection(logFile));
        _readEvents.push_back(Event(logFile));
        _writeEvents.push_back(Event(logFile));
        _connections[i]._readEvent = &_readEvents[i];
        _connections[i]._writeEvent = &_writeEvents[i];
        _spareConnections.push_back(&_connections[i]);
    }
}

Connection* ConnectionPool::getConnection()
{
    Connection*     connection;

    if (!_spareConnections.size())
        return (NULL);
    connection = _spareConnections.front();
    _spareConnections.pop_front();
    return (connection);
}

void ConnectionPool::returnConnection(Connection* connection)
{
    connection->reset();
    _spareConnections.push_front(connection);
}


//private, as usual

ConnectionPool::ConnectionPool() 
    : _spareConnMemPool(3){}
    
ConnectionPool::ConnectionPool(const ConnectionPool& copy) : _spareConnMemPool(3)
{
    (void)copy;
}


ConnectionPool& ConnectionPool::operator=(const ConnectionPool& assign)
{
    (void)assign;
    return (*this);
}