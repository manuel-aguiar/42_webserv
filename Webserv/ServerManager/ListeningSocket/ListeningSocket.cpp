/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/02 14:36:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"
#include "../ServerWorker/ServerWorker.hpp"
#include "../ConnectionManager/ConnectionManager.hpp"
#include "../EventManager/EventManager.hpp"
#include "../../Globals/Globals.hpp"
#include "../../Event/Event.hpp"
#include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

ListeningSocket::ListeningSocket(ServerWorker& worker, Globals* globals) :
	m_worker(worker),
	m_globals(globals)
{
	#if !defined(NDEBUG) && defined(DEBUG_CTOR)
		#include <iostream>
		if (globals)
			m_globals->m_debugFile->record("ListeningSocket Constructor Called");
		else
			std::cout << "ListeningSocket Constructor Called" << std::endl;
	#endif
}

ListeningSocket::~ListeningSocket()
{
	close();

#if !defined(NDEBUG) && defined(DEBUG_CTOR)
	#include <iostream>
	if (globals)
		m_globals->m_debugFile->record("ListeningSocket Destructor Called");
	else
		std::cout << "ListeningSocket Destructor Called" << std::endl;
#endif

}

int		ListeningSocket::open()
{
	int options;

	m_sockfd = ::socket(m_addr->sa_family, m_socktype, m_proto);
	//std::cout << "listener sockfd " << m_sockfd << std::endl;
	if (m_sockfd == -1)
	{
		m_globals->logStatus("socket(): " + std::string(strerror(errno)));
		return (0);
	}

	/*
		TODO: fcntl set nonblocking
	*/

	#ifdef SO_REUSEPORT
		options = SO_REUSEADDR | SO_REUSEPORT;
	#else
		options = SO_REUSEADDR;
	#endif

	if (::setsockopt(m_sockfd, SOL_SOCKET, options, &options, sizeof(options)) == -1)
	{
		m_globals->logStatus("setsockopt(): " + std::string(strerror(errno)));
		return (0);
	}

	if (!bind() || !listen())
		return (0);
	return (1);
}

int		ListeningSocket::bind()
{
	if (::bind(m_sockfd, m_addr, m_addrlen) == -1)
	{
		m_globals->logStatus("bind(): " + std::string(strerror(errno)));
		return (0);
	}
	return (1);
}

int		ListeningSocket::listen()
{
	if (::listen(m_sockfd, m_backlog) == -1)
	{
		m_globals->logStatus("listen(): " + std::string(strerror(errno)));
		return (0);
	}
	return (1);
}

void    ListeningSocket::accept()
{
	Connection*	connection;
	u_sockaddr	addr;
	t_socklen	addrlen;
	t_sockaddr*	addrrr;
	int			sockfd;


	connection = m_worker.accessConnManager().provideConnection();

	if (!connection)
	{
		std::cout << "       connection pool empty" << std::endl;
		return ;
	}

	connection->setListener(*this);
	addrlen = sizeof(addr);
	sockfd = ::accept(m_sockfd, &addr.sockaddr, &addrlen);


	if (sockfd == -1)
		goto NewConnection_Failure;

	connection->setSocket(sockfd);

	if (!FileDescriptor::setCloseOnExec_NonBlocking(sockfd))
		goto NewConnection_Failure;

	addrrr = (t_sockaddr*)connection->accessMemPool().allocate(addrlen, true);

	if (!addrrr)
		goto NewConnection_Failure;
	connection->setAddr(addrrr);

	std::memcpy(connection->accessAddr(), &addr, addrlen);
	connection->setAddrlen(addrlen);

	//listener passes protoModule to connection and calls the ProtoConnection initializer (ServerWorker sets these from configuration parsing)
	
	connection->setProtoModule(m_protoModule);
	m_initConnection(connection);

 //std::cout << "added conenction" << std::endl;
	if (!m_worker.accessEventManager().addEvent(
			connection->getSocket(),
			connection->accessReadEvent())
		)
		goto NewConnection_Failure;

	return ;

NewConnection_Failure:
	m_globals->logStatus("ListeningSocket::listener_Accept(): " + std::string(strerror(errno)));
	mf_close_accepted_connection(connection);

}

void    ListeningSocket::mf_close_accepted_connection(Connection* connection)
{
	if (connection->getSocket() != -1 && ::close(connection->getSocket()) == -1)
		m_globals->logStatus("close(): " + std::string(strerror(errno)));
	connection->setSocket(-1);
	connection->reset();
	m_worker.accessConnManager().returnConnection(connection);
}

void    ListeningSocket::closeConnection(Connection* connection)
{
	m_worker.accessEventManager().delEvent(connection->getSocket());
	mf_close_accepted_connection(connection);

}


void    ListeningSocket::close()
{
	if (::close(m_sockfd) == -1)
		m_globals->logStatus("close(): " + std::string(strerror(errno)));
}



//private
ListeningSocket::ListeningSocket() :
	m_worker(*((ServerWorker*)NULL)),  					//never do this, for real
	m_globals(NULL)
{

#if !defined(NDEBUG) && defined(DEBUG_CTOR)
	#include <iostream>
	if (globals)
		m_globals->m_debugFile->record("ListeningSocket Destructor Called");
	else
		std::cout << "ListeningSocket Destructor Called" << std::endl;
#endif

}
