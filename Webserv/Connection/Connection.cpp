/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/12 11:35:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Connection.hpp"
# include "../Event/Event.hpp"
# include "../Globals/LogFile/LogFile.hpp"
# include "../ServerManager/ListeningSocket/ListeningSocket.hpp"

Connection::Connection(Globals& globals) :
    m_memPool(*Nginx_MemoryPool::create(4096, 10)),
    m_globals(globals)
{

}

Connection::~Connection()
{
    m_memPool.destroy();
}

void    Connection::init()
{
    
}

void    Connection::reset()
{
    m_sockfd = -1;
    m_listener = NULL;
    m_memPool.reset();
}

void    Connection::read()
{

}


void    Connection::write()
{

}


// no copies, as usual
Connection::Connection(const Connection& other) :
    m_sockfd(other.m_sockfd),
    m_addr(other.m_addr),
    m_addrlen(other.m_addrlen),
    m_readEvent(other.m_readEvent),
    m_writeEvent(other.m_writeEvent),
    m_listener(other.m_listener),
    m_memPool(other.m_memPool),
    m_globals(other.m_globals)
{

}

Connection& Connection::operator=(const Connection& other) {(void)other; return (*this);}
