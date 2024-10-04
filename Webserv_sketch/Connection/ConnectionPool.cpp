/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/04 11:07:15 by mmaria-d         ###   ########.fr       */
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
    _connections(maxConnections),
    _readEvents(maxConnections),
    _writeEvents(maxConnections),
    _spareConnections(MPool_FixedElem<Connection*>(maxConnections))
{


    for (size_t i = 0; i < maxConnections; i++)
    {
        new (&_connections[i]) Connection(_globals);
        new (&_readEvents[i]) Event(_globals);
        new (&_writeEvents[i]) Event(_globals);
        
        _connections[i].init();
        _connections[i]._readEvent = &_readEvents[i];
        _connections[i]._writeEvent = &_writeEvents[i];
        _readEvents[i].setHandler_Function_and_Data(&HandlerFunction::connection_Read, &_connections[i]);
        _readEvents[i].setFlags(EPOLLIN);
        _writeEvents[i].setHandler_Function_and_Data(&HandlerFunction::connection_Write, &_connections[i]);
        _writeEvents[i].setFlags(EPOLLOUT);
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
    std::cout << "connection sent away" << _spareConnections.size() <<  std::endl;
    return (connection);
}

void ConnectionPool::returnConnection(Connection* connection)
{
    connection->reset();
    _spareConnections.push_front(connection);
    std::cout << "connection returned: " << _spareConnections.size() <<  std::endl;
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