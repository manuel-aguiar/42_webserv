/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionGetSetAccess.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 10:19:24 by manuel            #+#    #+#             */
/*   Updated: 2024/12/02 14:38:07 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Connection.hpp"

//getters
t_socket				Connection::getSocket() const
{
	return (m_sockfd);
}

const t_sockaddr*		Connection::getAddr() const
{
	return (m_addr);
}

t_socklen				Connection::getAddrlen() const
{
	return (m_addrlen);
}

const Event&			Connection::getReadEvent() const
{
	return (*m_readEvent);
}

const Event&			Connection::getWriteEvent() const
{
	return (*m_writeEvent);
}

const ListeningSocket&	Connection::getListener() const
{
	return (*m_listener);
}

const Globals&			Connection::getGlobals() const
{
	return (*m_globals);
}

const Nginx_MemoryPool&	Connection::getMemPool() const
{
	return (*m_memPool);
}


//setters
void					Connection::setSocket(const t_socket sockfd)
{
	m_sockfd = sockfd;
}

void					Connection::setAddr(const t_sockaddr* addr)
{
	m_addr = addr;
}

void					Connection::setAddrlen(const t_socklen addrlen)
{
	m_addrlen = addrlen;
}

void					Connection::setReadEvent(const Event& event)
{
	m_readEvent = &event;
}

void					Connection::setWriteEvent(const Event& event)
{
	m_writeEvent = &event;
}

void					Connection::setListener(const ListeningSocket& listener)
{
	m_listener = &listener;
}

void					Connection::setGlobals(const Globals* globals)
{
	m_globals = globals;
}

void					Connection::setMemPool(const Nginx_MemoryPool& memPool)
{
	m_memPool = &memPool;
}

void					Connection::setProtoConnection(const t_ptr_ProtoConnection connection)
{
	m_ptr_protoConnection = connection;
}

void					Connection::setProtoModule(const t_ptr_ProtoModule module)
{
	m_ptr_protoModule = module;
}


//accessors
t_ptr_ProtoConnection	Connection::accessProtoConnection()
{
	return (m_ptr_protoConnection);
}

t_ptr_ProtoModule		Connection::accessProtoModule()
{
	return (m_ptr_protoModule);
}

Nginx_MemoryPool&		Connection::accessMemPool()
{
	return (*m_memPool);
}

t_sockaddr*		Connection::accessAddr()
{
	return (m_addr);
}

