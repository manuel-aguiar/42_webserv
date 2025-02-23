

// Project headers
# include "Listener.hpp"
# include "../ConnInfo/ConnInfo.hpp"
# include "../../GenericUtils/FileDescriptor/FileDescriptor.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstring> // memset

// C headers
# include <unistd.h>

Listener::Listener(const Ws::BindInfo& socketInfo) : 
	m_info(Ws::FD_NONE, socketInfo) {}

Listener::~Listener() {}

int	Listener::open()
{
	int 				options;
	int 				sockfd;

	sockfd = ::socket(m_info.bind.family, m_info.bind.socktype, m_info.bind.proto);

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
	if (::bind(sockfd, (struct sockaddr*)(&m_info.bind.addr), m_info.bind.addrlen) == -1)
	{
		::close(sockfd);
		return (0);
	}
	if (::listen(sockfd, m_info.bind.backlog) == -1)
	{
		::close(sockfd);
		return (0);
	}

	m_info.sockfd = sockfd;
	return (1);
}

void	Listener::close()
{
	if (m_info.sockfd == Ws::FD_NONE)
		return ;
	::close(m_info.sockfd);
	m_info.sockfd = Ws::FD_NONE;
}

int		Listener::accept(ConnInfo& accept)
{
	int 				sockfd;
	Ws::BindInfo&		acceptInfo = accept.bind;

	acceptInfo.addrlen = m_info.bind.addrlen;
	sockfd = ::accept(m_info.sockfd, (struct sockaddr*)(&acceptInfo.addr), &acceptInfo.addrlen);

	if (sockfd == -1)
		return (-1);
		
	if (!FileDescriptor::setCloseOnExec_NonBlocking(sockfd))
	{
		::close(sockfd);
		std::memset(&acceptInfo, 0, sizeof(acceptInfo));
		return (-1);
	}

	acceptInfo.family = m_info.bind.family;
	acceptInfo.socktype = m_info.bind.socktype;
	acceptInfo.proto = m_info.bind.proto;

	// ovewritting the ephemeral port with that of the listeningConnInfo, that's what matters for us
	switch (acceptInfo.family)
	{
		case AF_INET:
			acceptInfo.addr.sockaddr_in.sin_port = m_info.bind.addr.sockaddr_in.sin_port;
			break;
		default:
			ASSERT_EQUAL(1, 0, "Accepter::accept(), unsupported family");
	}

	accept.sockfd = sockfd;

	return (1);
}

const ConnInfo&
Listener::getConnInfo() const
{
	return (m_info);
}

Listener::Listener(const Listener& copy) : 
	m_info(copy.m_info)
{
	ASSERT_EQUAL (copy.m_info.sockfd, (Ws::fd)Ws::FD_NONE, "Listener::Listener(), copy constructor, copy-from socket fd not set to none");
}

Listener& Listener::operator=(const Listener& assign)
{
	if (this == &assign)
		return (*this);

	ASSERT_EQUAL (m_info.sockfd, (Ws::fd)Ws::FD_NONE, "Listener::Listener(), copy assignment, copy-to socket fd not set to none");
	ASSERT_EQUAL (assign.m_info.sockfd, (Ws::fd)Ws::FD_NONE, "Listener::Listener(), copy assignment, copy-from socket fd not set to none");

	m_info.bind = assign.m_info.bind;

	return (*this);
}
