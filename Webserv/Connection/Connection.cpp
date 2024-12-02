/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/02 10:41:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Connection.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"
# include "../../../Event/Event.hpp"
# include "../../../Globals/LogFile/LogFile.hpp"

Connection::Connection(Globals* globals) :
    m_globals(globals)
{

}

Connection::~Connection()
{

}

void    Connection::init()
{
    m_memPool = Nginx_MemoryPool::create(4096, 10);
}

void    Connection::reset()
{
    m_sockfd = -1;
    m_listener = NULL;
    m_memPool->reset();
}

void    Connection::read()
{

}


void    Connection::write()
{

}


// no copies, as usual
Connection::Connection(const Connection& other) { (void)other;}
Connection& Connection::operator=(const Connection& other) {return (*this);}
