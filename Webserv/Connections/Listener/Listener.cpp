

// Project headers
# include "Listener.hpp"
# include "../Socket/Socket.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C headers
# include <unistd.h>

Listener::Listener(const Ws::BindInfo& socketInfo) : 
	m_sockfd(Ws::FD_NONE),
	m_bindInfo(socketInfo) {}

Listener::~Listener() {}

int	Listener::open()
{
	int 				options;
	int 				sockfd;

	sockfd = ::socket(m_bindInfo.family, m_bindInfo.socktype, m_bindInfo.proto);

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
	if (::bind(sockfd, (struct sockaddr*)(&m_bindInfo.addr), m_bindInfo.addrlen) == -1)
	{
		::close(sockfd);
		return (0);
	}
	if (::listen(sockfd, m_bindInfo.backlog) == -1)
	{
		::close(sockfd);
		return (0);
	}

	m_sockfd = sockfd;
	return (1);
}

void	Listener::close()
{
	if (m_sockfd == Ws::FD_NONE)
		return ;
	::close(m_sockfd);
	m_sockfd = Ws::FD_NONE;
}

int		Listener::accept(ConnInfo& accept)
{
	int 				sockfd;
	Ws::BindInfo&		modifyInfo = accept.modifyBindInfo();

	modifyInfo.addrlen = m_bindInfo.addrlen;
	sockfd = ::accept(m_sockfd, (struct sockaddr*)(&modifyInfo.addr), &modifyInfo.addrlen);

	if (sockfd == -1)
		return (-1);
		
	if (!FileDescriptor::setCloseOnExec_NonBlocking(sockfd))
	{
		::close(sockfd);
		modifyInfo = (Ws::BindInfo){};
		return (-1);
	}

	modifyInfo.family = m_bindInfo.family;
	modifyInfo.socktype = m_bindInfo.socktype;
	modifyInfo.proto = m_bindInfo.proto;

	// ovewritting the ephemeral port with that of the listeningSocket, that's what matters for us
	switch (modifyInfo.family)
	{
		case AF_INET:
			modifyInfo.addr.sockaddr_in.sin_port = m_bindInfo.addr.sockaddr_in.sin_port;
			break;
		default:
			ASSERT_EQUAL(1, 0, "Accepter::accept(), unsupported family");
	}

	accept.setSockFd(sockfd);

	return (1);
}

Ws::fd
Listener::getSockFd()
{
	return (m_sockfd);
}

const Ws::BindInfo&
Listener::getBindInfo()
{
	return (m_bindInfo);
}

Listener::Listener(const Listener& copy) : 
	m_sockfd(copy.m_sockfd),
	m_bindInfo(copy.m_bindInfo)
{
	ASSERT_EQUAL (copy.m_sockfd, (Ws::fd)Ws::FD_NONE, "Listener::Listener(), copy constructor, copy-from socket fd not set to none");
}

Listener& Listener::operator=(const Listener& assign)
{
	if (this == &assign)
		return (*this);

	ASSERT_EQUAL (m_sockfd, (Ws::fd)Ws::FD_NONE, "Listener::Listener(), copy assignment, copy-to socket fd not set to none");
	ASSERT_EQUAL (assign.m_sockfd, (Ws::fd)Ws::FD_NONE, "Listener::Listener(), copy assignment, copy-from socket fd not set to none");

	m_bindInfo = assign.m_bindInfo;

	return (*this);
}