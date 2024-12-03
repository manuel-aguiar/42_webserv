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
    for (size_t i = 0; i < m_connections.size(); i++)
        m_connections[i].accessMemPool().destroy();
}

ConnectionManager::ConnectionManager(size_t maxConnections, Nginx_MemoryPool* pool, Globals* globals) :
    m_maxConnections(maxConnections),
    m_connections(*pool),
    m_readEvents(*pool),
    m_writeEvents(*pool),
    m_spareConnections(*pool),
    m_globals(globals)
{
    m_connections.reserve(m_maxConnections);
    m_readEvents.reserve(m_maxConnections);
    m_writeEvents.reserve(m_maxConnections);
    m_spareConnections.resize(m_maxConnections);

    for (size_t i = 0; i < maxConnections; i++)
    {
        new (&m_connections[i]) Connection(m_globals);
        new (&m_readEvents[i]) Event;
        new (&m_writeEvents[i]) Event;

        m_connections[i].init();
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
    std::cout << "connection sent away: " << m_spareConnections.size() <<  std::endl;
    return (connection);
}

void ConnectionManager::returnConnection(Connection* connection)
{
    connection->reset();
    m_spareConnections.push_front(connection);
    std::cout << "connection returned: " << m_spareConnections.size() <<  std::endl;
}


//private, as usual

ConnectionManager::ConnectionManager() {}

ConnectionManager::ConnectionManager(const ConnectionManager& copy)
{
    (void)copy;
}


ConnectionManager& ConnectionManager::operator=(const ConnectionManager& assign)
{
    (void)assign;
    return (*this);
}
