

// Project headers
# include "Listener.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C headers
# include <unistd.h>

Listener::Listener(int backlog) : 
	m_backlog(backlog) {}

Listener::~Listener() {}

int	Listener::open(Socket& listen)
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

void	Listener::close(Socket& listen)
{
	ASSERT_EQUAL(listen.getSockFd() != -1, true, "Listener::close(), socket already closed");
	::close(listen.getSockFd());
	listen.setSockFd(-1);
}

int		Listener::accept(const Socket& listen, Socket& accept)
{
	int 				sockfd;
	const Ws::BindInfo&	m_info = listen.getBindInfo();
	Ws::BindInfo&		modifyInfo = accept.modifyBindInfo();

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
	accept.setSockFd(sockfd);

	return (1);
}

Listener::Listener(const Listener& copy) : 
	m_backlog(copy.m_backlog) {}

Listener& Listener::operator=(const Listener& assign)
{
	if (this == &assign)
		return (*this);

	m_backlog = assign.m_backlog;

	return (*this);
}