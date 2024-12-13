/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockListeningSocket.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:52:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 16:34:45 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ListeningSocket/ListeningSocket.hpp"
#include "../ServerWorker/ServerWorker.hpp"
#include "../ConnectionManager/ConnectionManager.hpp"
#include "../EventManager/EventManager.hpp"
#include "../../Globals/Globals.hpp"
#include "../../Event/Event.hpp"
#include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

ListeningSocket::ListeningSocket(ServerWorker& worker, Nginx_MemoryPool& memPool, const t_addrinfo& addrInfo, int backlog, Globals& globals) :
	m_backlog(backlog),
	m_worker(worker),
	m_globals(globals)
{
	size_t canonnameLen;

	#if !defined(NDEBUG) && defined(DEBUG_CTOR)
		#include <iostream>
		if (globals)
			m_globals.m_debugFile->record("ListeningSocket Constructor Called");
		else
			std::cout << "ListeningSocket Constructor Called" << std::endl;
	#endif

	std::memcpy(&m_addrInfo, &addrInfo, sizeof(addrInfo));
	m_addrInfo.ai_next = NULL;

	m_addrInfo.ai_addr = (t_sockaddr *)memPool.allocate(m_addrInfo.ai_addrlen, true);
	std::memcpy(m_addrInfo.ai_addr, addrInfo.ai_addr, m_addrInfo.ai_addrlen);

	// just convert the port to host byte order in place
	// this should require some form of switch statement to detect various address types as IPV6
	((t_sockaddr_in *)m_addrInfo.ai_addr)->sin_port = ::ntohs(((t_sockaddr_in *)m_addrInfo.ai_addr)->sin_port);

    m_addrInfo.ai_canonname = NULL;
	canonnameLen = addrInfo.ai_canonname ? std::strlen(addrInfo.ai_canonname) : 0;
    
	if (canonnameLen)
	{
		m_addrInfo.ai_canonname = (char *)memPool.allocate(canonnameLen + 1, true);
		std::memcpy(m_addrInfo.ai_canonname, addrInfo.ai_canonname, canonnameLen + 1);
	}

	m_event.setFd_Data_Handler_Flags(m_sockfd, this, &ListeningSocket::EventAccept, EPOLLIN | EPOLLET);
}

ListeningSocket::~ListeningSocket()
{
	close();

	#if !defined(NDEBUG) && defined(DEBUG_CTOR)
		#include <iostream>
		if (globals)
			m_globals.m_debugFile->record("ListeningSocket Destructor Called");
		else
			std::cout << "ListeningSocket Destructor Called" << std::endl;
	#endif
}

int		ListeningSocket::open()
{
	int options;

	m_sockfd = ::socket(m_addrInfo.ai_family, m_addrInfo.ai_socktype, m_addrInfo.ai_protocol);
	//std::cout << "listener sockfd " << m_sockfd << std::endl;
	if (m_sockfd == -1)
	{
		m_globals.logError("ListeningSocket::open, socket(): " + std::string(strerror(errno)));
		return (0);
	}

	if (!FileDescriptor::setCloseOnExec_NonBlocking(m_sockfd))
	{
		m_globals.logError("ListeningSocket::open(), setCloseOnExec_NonBlocking(): " + std::string(strerror(errno)));
		return (0);
	}	

	#ifdef SO_REUSEPORT
		options = SO_REUSEADDR | SO_REUSEPORT;
	#else
		options = SO_REUSEADDR;
	#endif

	if (::setsockopt(m_sockfd, SOL_SOCKET, options, &options, sizeof(options)) == -1)
	{
		m_globals.logError("ListeningSocket::open(), setsockopt(): " + std::string(strerror(errno)));
		return (0);
	}

	if (!mf_bind() || !mf_listen())
		return (0);
	return (1);
}


/*
	Accepts a socket conenction on the ListeningSocket. Will be called by the EventManager
	via the EventAccept function pointer, and by the ServerWorker when a Connection instance
	is returned to try and recycle. 
*/
int    ListeningSocket::accept()
{
	Connection*	connection;
	u_sockaddr	addr;
	t_socklen	addrlen;
	t_sockaddr*	addrrr;
	int			sockfd;


	connection = m_worker.provideConnection();

	if (!connection)
	{
		m_worker.addPendingAccept(*this);
		return (0);
	}

	connection->setListener(*this);
	addrlen = sizeof(addr);
	sockfd = ::accept(m_sockfd, &addr.sockaddr, &addrlen);


	if (sockfd == -1)
	{
		
		if (!(errno == EAGAIN || errno == EWOULDBLOCK))
		{
			/* there was backlog but accept failed */
			m_globals.logError("ListeningSocket::accept(): " + std::string(strerror(errno)));
		}
		m_worker.returnConnection(*connection);
		return (0);		
	}

	connection->setSocket(sockfd);

	if (!FileDescriptor::setCloseOnExec_NonBlocking(sockfd))
	{
		m_globals.logError("ListeningSocket::accept(), setCloseOnExec_NonBlocking(): " + std::string(strerror(errno)));
		return (mf_close_accepted_connection(*connection));
	}

	addrrr = (t_sockaddr*)connection->accessMemPool().allocate(addrlen, true);

	if (!addrrr)
	{
		m_globals.logError("ListeningSocket::accept(), memorypool " + std::string(strerror(errno)));
		return (mf_close_accepted_connection(*connection));
	}
	connection->setAddr(addrrr);

	std::memcpy(connection->accessAddr(), &addr, addrlen);
	connection->setAddrlen(addrlen);
	connection->setProtoModule(m_protoModule);
	m_initConnection(connection);

	if (!m_worker.accessEventManager().addEvent(connection->getReadEvent()))
		return (mf_close_accepted_connection(*connection));

	return (1);
}

int    ListeningSocket::mf_close_accepted_connection(Connection& connection)
{
	if (connection.getSocket() != -1 && ::close(connection.getSocket()) == -1)
		m_globals.logError("close(): " + std::string(strerror(errno)));
	m_worker.returnConnection(connection);
	return (0);
}

void    ListeningSocket::closeConnection(Connection& connection)
{
	m_worker.accessEventManager().delEvent(connection.getReadEvent());
	mf_close_accepted_connection(connection);
}


void    ListeningSocket::close()
{
	if (::close(m_sockfd) == -1)
		m_globals.logError("close(): " + std::string(strerror(errno)));
}



//private
ListeningSocket::ListeningSocket() :
	m_worker(*((ServerWorker*)NULL)),  					//never do this, for real
	m_globals(*((Globals*)NULL))
{

	#if !defined(NDEBUG) && defined(DEBUG_CTOR)
		#include <iostream>
		if (globals)
			m_globals.m_debugFile->record("ListeningSocket Destructor Called");
		else
			std::cout << "ListeningSocket Destructor Called" << std::endl;
	#endif

}

void ListeningSocket::EventAccept(Event& event)
{
	ListeningSocket*	listener;

	listener = reinterpret_cast<ListeningSocket*>(event.getData());
	listener->accept();
}


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

void						ListeningSocket::setProtoModule					(const t_ptr_ProtoModule& module)
{
	m_protoModule = module;
}

void						ListeningSocket::setInitProtocolConnection		(const t_func_initProtoConn& func)
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

int		ListeningSocket::mf_bind()
{
	if (::bind(m_sockfd, (struct sockaddr*)m_addrInfo.ai_addr, m_addrInfo.ai_addrlen) == -1)
	{
		m_globals.logError("ListeningSocket::bind(), bind(): " + std::string(strerror(errno)));
		return (0);
	}
	return (1);
}

int		ListeningSocket::mf_listen()
{
	if (::listen(m_sockfd, m_backlog) == -1)
	{
		m_globals.logError("ListeningSocket::listen(): listen():" + std::string(strerror(errno)));
		return (0);
	}
	return (1);
