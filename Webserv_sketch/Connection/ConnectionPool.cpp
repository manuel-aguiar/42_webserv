/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 15:05:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionPool.hpp"

ConnectionPool::ConnectionPool() {}
ConnectionPool::~ConnectionPool() {}




Connection* ConnectionPool::getConnection()
{
    Connection*     connection;

    if (_spareConnections.size())
    {
        connection = _spareConnections.front();
        _spareConnections.pop_front();
    }
    else
    {
        connection = _connectionAlloc.allocate();
        connection->_readEvent = _eventAlloc.allocate();
        connection->_writeEvent = _eventAlloc.allocate();
    }
        
    return (connection);
}

void ConnectionPool::returnConnection(Connection* connection)
{
    connection->reset();
    _spareConnections.push_front(connection);
}


void ConnectionPool::destroyConnection(Connection* connection)
{
    _connectionAlloc.deallocate(connection);
}

//private, as usual
ConnectionPool::ConnectionPool(const ConnectionPool& copy)
{
    (void)copy;
}

ConnectionPool& ConnectionPool::operator=(const ConnectionPool& assign)
{
    (void)assign;
    return (*this);
}