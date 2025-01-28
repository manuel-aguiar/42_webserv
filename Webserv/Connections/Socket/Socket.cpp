

#include "Socket.hpp"

Socket::Socket(const Ws::Sock::fd sockfd, const Ws::BindInfo& info) : 
	m_sockfd(sockfd),
	m_info(info) {}

Socket::Socket() : 
	m_sockfd(-1),
	m_info((Ws::BindInfo){}) {}

Socket::~Socket() {}

Socket::Socket(const Socket& copy) : 
	m_sockfd(copy.m_sockfd),
	m_info(copy.m_info) {}

Socket& Socket::operator=(const Socket& assign)
{
	if (this == &assign)
		return (*this);
	m_sockfd = assign.m_sockfd;
	m_info = assign.m_info;
	return (*this);
}

Ws::Sock::fd
Socket::getSockFd() const
{
	return (m_sockfd);
}

const Ws::BindInfo&
Socket::getBindInfo() const
{
	return (m_info);
}


void
Socket::setSockFd(const Ws::Sock::fd sockfd)
{
	m_sockfd = sockfd;
}

void
Socket::setBindInfo(const Ws::BindInfo& info)
{
	m_info = info;
}

Ws::BindInfo&
Socket::modifyBindInfo()
{
	return (m_info);
}

void
Socket::reset()
{
	m_sockfd = -1;
	m_info = (Ws::BindInfo){};
}