/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/09 15:11:42 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListeningSocket.hpp"
#include "../ServerWorker/ServerWorker.hpp"
#include "../ConnectionManager/ConnectionManager.hpp"
#include "../EventManager/EventManager.hpp"
#include "../../Globals/Globals.hpp"
#include "../../Event/Event.hpp"
#include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

ListeningSocket::ListeningSocket(ServerWorker& worker, const t_addrinfo& addrInfo, Globals* globals) :
	m_worker(worker),
	m_globals(globals)
{
	size_t canonnameLen;

	#if !defined(NDEBUG) && defined(DEBUG_CTOR)
		#include <iostream>
		if (globals)
			m_globals->m_debugFile->record("ListeningSocket Constructor Called");
		else
			std::cout << "ListeningSocket Constructor Called" << std::endl;
	#endif

	std::memcpy(&m_addrInfo, &addrInfo, sizeof(addrInfo));
	m_addrInfo.ai_next = NULL;

	m_addrInfo.ai_addr = (t_sockaddr *)m_worker.accessMemPool().allocate(m_addrInfo.ai_addrlen, true);
	std::memcpy(m_addrInfo.ai_addr, addrInfo.ai_addr, m_addrInfo.ai_addrlen);

	// just convert the port to host byte order in place
	// this should require some form of switch statement to detect various address types as IPV6
	((t_sockaddr_in *)m_addrInfo.ai_addr)->sin_port = ::ntohs(((t_sockaddr_in *)m_addrInfo.ai_addr)->sin_port);

	canonnameLen = addrInfo.ai_canonname ? std::strlen(addrInfo.ai_canonname) : 0;

	if (!canonnameLen)
		m_addrInfo.ai_canonname = NULL;
	else
	{
		m_addrInfo.ai_canonname = (char *)m_worker.accessMemPool().allocate(canonnameLen + 1, true);
		std::memcpy(m_addrInfo.ai_canonname, addrInfo.ai_canonname, canonnameLen + 1);
	}

	m_event.setFd_Data_Handler_Flags(m_sockfd, this, &ListeningSocket::EventAccept, EPOLLIN);
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

	m_sockfd = ::socket(m_addrInfo.ai_family, m_addrInfo.ai_socktype, m_proto);
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
	if (::bind(m_sockfd, (struct sockaddr*)m_addrInfo.ai_addr, m_addrInfo.ai_addrlen) == -1)
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
		/*
			No connections available, add listener to the pending accept list at the worker
		*/
		m_worker.addPendingAccept(this);
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

	//initializes the relevant protocol structure and prepares events for the Connection before
	// being added to epoll for monitoring, below.
	m_initConnection(connection);

	if (!m_worker.accessEventManager().addEvent(connection->getReadEvent()))
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
	m_worker.accessEventManager().delEvent(connection->getReadEvent());
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
