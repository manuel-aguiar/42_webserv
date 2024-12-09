/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocketGetSetAccess.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 10:33:06 by manuel            #+#    #+#             */
/*   Updated: 2024/12/09 15:12:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"

/*
	struct addrinfo {
		int              ai_flags;
		int              ai_family;
		int              ai_socktype;
		int              ai_protocol;
		socklen_t        ai_addrlen;
		struct sockaddr *ai_addr;
		char            *ai_canonname;
		struct addrinfo *ai_next;
	};

*/

const ServerWorker&			ListeningSocket::getWorker()						const
{
	return (m_worker);
}

t_socket					ListeningSocket::getSocket()						const
{
	return (m_sockfd);
}

int							ListeningSocket::getSockType()					const
{
	return (m_addrInfo.ai_socktype);
}

int							ListeningSocket::getProtocol()					const
{
	return (m_addrInfo.ai_protocol);
}

const t_sockaddr*			ListeningSocket::getAddr()						const
{
	return ((t_sockaddr*)m_addrInfo.ai_addr);
}

t_socklen					ListeningSocket::getAddrlen()					const
{
	return (m_addrInfo.ai_addrlen);
}

t_port						ListeningSocket::getPort()						const
{
	return (((t_sockaddr_in*)m_addrInfo.ai_addr)->sin_port);
}

int							ListeningSocket::getBacklog()					const
{
	return (m_backlog);
}

const Event&				ListeningSocket::getEvent()						const
{
	return (m_event);
}

void						ListeningSocket::setProtoModule					(const t_ptr_ProtoModule module)
{
	m_protoModule = module;
}

void						ListeningSocket::setInitProtocolConnection		(const t_func_initProtoConn func)
{
	m_initConnection = func;
}


// accessors
ServerWorker&				ListeningSocket::accessWorker()
{
	return (m_worker);
}

Event&						ListeningSocket::accessEvent()
{
	return (m_event);
}

