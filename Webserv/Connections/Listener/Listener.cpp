

// Project headers
# include "Listener.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C headers
# include <unistd.h>

Listener::Listener(int backlog, const Ws::BindInfo& info) : 
	m_socket(-1, info),
	m_backlog(backlog) {}

Listener::~Listener() {}

int	Listener::open()
{
	int 				options;
	int 				sockfd;
	const Ws::BindInfo& m_info = m_socket.getBindInfo();

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

	m_socket.setSockFd(sockfd);
	return (1);
}

void	Listener::close()
{
	ASSERT_EQUAL(m_socket.getSockFd() != -1, true, "Listener::close(), socket already closed");
	::close(m_socket.getSockFd());
	m_socket.setSockFd(-1);
}

int		Listener::accept(Socket& fill)
{
	int 				sockfd;
	const Ws::BindInfo& m_info = m_socket.getBindInfo();
	Ws::BindInfo&		modifyInfo = fill.modifyBindInfo();

	sockfd = ::accept(m_socket.getSockFd(), (struct sockaddr*)(&modifyInfo.addr), &modifyInfo.addrlen);

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
	fill.setSockFd(sockfd);

	return (1);
}

Listener::Listener(const Listener& copy) : 
	m_socket(copy.m_socket),
	m_backlog(copy.m_backlog) {}

Listener& Listener::operator=(const Listener& assign)
{
	if (this == &assign)
		return (*this);

	m_socket = assign.m_socket;
	m_backlog = assign.m_backlog;

	return (*this);
}