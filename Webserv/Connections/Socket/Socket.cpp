

#include "Socket.hpp"

ConnInfo::ConnInfo(const Ws::Sock::fd sockfd, const Ws::BindInfo& info) : 
	m_sockfd(sockfd),
	m_info(info) {}

ConnInfo::ConnInfo() : 
	m_sockfd(Ws::FD_NONE),
	m_info((Ws::BindInfo){}) {}

ConnInfo::~ConnInfo() {}

ConnInfo::ConnInfo(const ConnInfo& copy) : 
	m_sockfd(copy.m_sockfd),
	m_info(copy.m_info) {}

ConnInfo& ConnInfo::operator=(const ConnInfo& assign)
{
	if (this == &assign)
		return (*this);

	m_sockfd = assign.m_sockfd;
	m_info = assign.m_info;
	
	return (*this);
}

Ws::Sock::fd
ConnInfo::getSockFd() const
{
	return (m_sockfd);
}

const Ws::BindInfo&
ConnInfo::getBindInfo() const
{
	return (m_info);
}


void
ConnInfo::setSockFd(const Ws::Sock::fd sockfd)
{
	m_sockfd = sockfd;
}

void
ConnInfo::setBindInfo(const Ws::BindInfo& info)
{
	m_info = info;
}

Ws::BindInfo&
ConnInfo::modifyBindInfo()
{
	return (m_info);
}

void
ConnInfo::reset()
{
	m_sockfd = Ws::FD_NONE;
	m_info = (Ws::BindInfo){};
}