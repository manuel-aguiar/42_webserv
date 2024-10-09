/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionPool.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:13:44 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConnectionPool.hpp"
#include "../Event/HandlerFunction.hpp"

ConnectionPool::~ConnectionPool() 
{
    for (size_t i = 0; i < m_connections.size(); i++)
        m_connections[i].m_memPool->destroy();
}

ConnectionPool::ConnectionPool(Globals* globals, size_t maxConnections) : 
    m_globals(globals), 
    m_maxConnections(maxConnections),
    m_connections(maxConnections),
    m_readEvents(maxConnections),
    m_writeEvents(maxConnections),
    m_spareConnections(MPool_FixedElem<Connection*>(maxConnections))
{


    for (size_t i = 0; i < maxConnections; i++)
    {
        new (&m_connections[i]) Connection(m_globals);
        new (&m_readEvents[i]) Event(m_globals);
        new (&m_writeEvents[i]) Event(m_globals);
        
        m_connections[i].init();
        m_connections[i].m_readEvent = &m_readEvents[i];
        m_connections[i].m_writeEvent = &m_writeEvents[i];
        m_readEvents[i].setHandler_Function_and_Data(&HandlerFunction::connection_Read, &m_connections[i]);
        m_readEvents[i].setFlags(EPOLLIN);
        m_writeEvents[i].setHandler_Function_and_Data(&HandlerFunction::connection_Write, &m_connections[i]);
        m_writeEvents[i].setFlags(EPOLLOUT);
        m_spareConnections.push_back(&m_connections[i]);
    }
}

Connection* ConnectionPool::getConnection()
{
    Connection*     connection;

    if (!m_spareConnections.size())
        return (NULL);
    connection = m_spareConnections.front();
    m_spareConnections.pop_front();
    std::cout << "connection sent away: " << m_spareConnections.size() <<  std::endl;
    return (connection);
}

void ConnectionPool::returnConnection(Connection* connection)
{
    connection->reset();
    m_spareConnections.push_front(connection);
    std::cout << "connection returned: " << m_spareConnections.size() <<  std::endl;
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