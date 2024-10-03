/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 12:55:12 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionPool.hpp"
#include "../Event/HandlerFunction.hpp"

ConnectionPool::~ConnectionPool() 
{
    for (size_t i = 0; i < _connections.size(); i++)
        _connections[i]._memPool->destroy();
}

ConnectionPool::ConnectionPool(Globals* globals, size_t maxConnections) : 
    _globals(globals), 
    _maxConnections(maxConnections),
    _spareConnections(MPool_FixedElem<Connection*>(maxConnections))
{
    _connections.reserve(maxConnections);
    _readEvents.reserve(maxConnections);
    _writeEvents.reserve(maxConnections);

    for (size_t i = 0; i < maxConnections; i++)
    {
        _connections.push_back(Connection(_globals));
        _readEvents.push_back(Event(_globals));
        _writeEvents.push_back(Event(_globals));
        _connections[i].init();
        _connections[i]._readEvent = &_readEvents[i];
        _connections[i]._writeEvent = &_writeEvents[i];
        _readEvents[i].setHandler_Function_and_Data(&HandlerFunction::connection_Read, &_connections[i]);
        _readEvents[i].setHandler_Function_and_Data(&HandlerFunction::connection_Write, &_connections[i]);
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

ConnectionPool::ConnectionPool() {}
    
ConnectionPool::ConnectionPool(const ConnectionPool& copy)
{
    (void)copy;
}


ConnectionPool& ConnectionPool::operator=(const ConnectionPool& assign)
{
    (void)assign;
    return (*this);
}