

// Project headers
# include "Accepter.hpp"
# include "../Socket/Socket.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C headers
# include <unistd.h>

Accepter::Accepter(int backlog) : 
	m_backlog(backlog) {}

Accepter::~Accepter() {}

int	Accepter::open(Socket& listen)
{
	int 				options;
	int 				sockfd;
	const Ws::BindInfo& m_info = listen.getBindInfo();

	sockfd = ::socket(m_info.family, m_info.socktype, m_info.proto);

	if (sockfd == -1)
		return (0);

	if (!FileDescriptor::setCloseOnExec_NonBlocking(sockfd))
	{
		::close(sockfd);
		return (0);
	}

	#ifdef SO_REUSEPORT
		options = SO_REUSEADDR | SO_REUSEPORT;
	#else
		options = SO_REUSEADDR;
	#endif

	if (::setsockopt(sockfd, SOL_SOCKET, options, &options, sizeof(options)) == -1)
	{
		::close(sockfd);
		return (0);
	}	
	if (::bind(sockfd, (struct sockaddr*)(&m_info.addr), m_info.addrlen) == -1)
	{
		::close(sockfd);
		return (0);
	}
	if (::listen(sockfd, m_backlog) == -1)
	{
		::close(sockfd);
		return (0);
	}

	listen.setSockFd(sockfd);
	return (1);
}

void	Accepter::close(Socket& listen)
{
	ASSERT_EQUAL(listen.getSockFd() != -1, true, "Accepter::close(), socket already closed");
	::close(listen.getSockFd());
	listen.setSockFd(-1);
}

int		Accepter::accept(const Socket& listen, Socket& accept)
{
	int 				sockfd;
	const Ws::BindInfo&	m_info = listen.getBindInfo();
	Ws::BindInfo&		modifyInfo = accept.modifyBindInfo();

	modifyInfo.addrlen = m_info.addrlen;
	sockfd = ::accept(listen.getSockFd(), (struct sockaddr*)(&modifyInfo.addr), &modifyInfo.addrlen);

	if (sockfd == -1)
		return (0);
		
	if (!FileDescriptor::setCloseOnExec_NonBlocking(sockfd))
	{
		::close(sockfd);
		return (0);
	}

	modifyInfo.family = m_info.family;
	modifyInfo.socktype = m_info.socktype;
	modifyInfo.proto = m_info.proto;

	// ovewritting the ephemeral port with that of the listeningSocket, that's what matters for us
	switch (modifyInfo.family)
	{
		case AF_INET:
			modifyInfo.addr.sockaddr_in.sin_port = m_info.addr.sockaddr_in.sin_port;
			break;
		default:
			ASSERT_EQUAL(1, 0, "Accepter::accept(), unsupported family");
	}

	accept.setSockFd(sockfd);

	return (1);
}

Accepter::Accepter(const Accepter& copy) : 
	m_backlog(copy.m_backlog) {}

Accepter& Accepter::operator=(const Accepter& assign)
{
	if (this == &assign)
		return (*this);

	m_backlog = assign.m_backlog;

	return (*this);
}