/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/14 09:39:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ConnectionManager.hpp"
# include "../../Event/Event.hpp"

ConnectionManager::~ConnectionManager()
{

}

ConnectionManager::ConnectionManager(size_t maxConnections, Nginx_MemoryPool& borrowedPool, Globals& globals) :
    m_maxConnections(maxConnections),
    m_connections(Nginx_PoolAllocator<Connection>(&borrowedPool)),
    m_readEvents(Nginx_PoolAllocator<Event>(&borrowedPool)),
    m_writeEvents(Nginx_PoolAllocator<Event>(&borrowedPool)),
    m_spareConnections(Nginx_MPool_FixedElem<Connection*>(&borrowedPool, maxConnections)),
    m_globals(globals)
{
    
    m_connections.reserve(m_maxConnections);
    m_readEvents.reserve(m_maxConnections);
    m_writeEvents.reserve(m_maxConnections);

    for (size_t i = 0; i < maxConnections; i++)
    {
        m_connections.emplace_back(m_globals);
        m_readEvents.emplace_back();
        m_writeEvents.emplace_back();

        m_connections[i].setReadEvent(m_readEvents[i]);
        m_connections[i].setWriteEvent(m_writeEvents[i]);

        m_spareConnections.push_back(&m_connections[i]);
    }

}

Connection* ConnectionManager::provideConnection()
{
    Connection*     connection;

    if (!m_spareConnections.size())
        return (NULL);
    connection = m_spareConnections.front();
    m_spareConnections.pop_front();
    return (connection);
}

void ConnectionManager::returnConnection(Connection* connection)
{
    connection->reset();
    m_spareConnections.push_front(connection);
}


//private, as usual

ConnectionManager::ConnectionManager(const ConnectionManager& copy) :
    m_maxConnections(copy.m_maxConnections),
    m_connections(copy.m_connections),
    m_readEvents(copy.m_readEvents),
    m_writeEvents(copy.m_writeEvents),
    m_spareConnections(copy.m_spareConnections),
    m_globals(copy.m_globals)
{(void)copy;}
ConnectionManager& ConnectionManager::operator=(const ConnectionManager& assign) {(void)assign; return (*this);}
