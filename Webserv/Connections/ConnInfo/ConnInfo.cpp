

#include "ConnInfo.hpp"
#include <cstring> // memset

ConnInfo::ConnInfo(const Ws::Sock::fd sockfd, const Ws::BindInfo& info) : 
	sockfd(sockfd),
	bind(info) {}

ConnInfo::ConnInfo() : 
	sockfd(Ws::FD_NONE),
	bind()
{
	std::memset(&bind, 0, sizeof(bind));
}

ConnInfo::~ConnInfo() {}

ConnInfo::ConnInfo(const ConnInfo& copy) : 
	sockfd(copy.sockfd),
	bind(copy.bind) {}

ConnInfo& ConnInfo::operator=(const ConnInfo& assign)
{
	if (this == &assign)
		return (*this);

	sockfd = assign.sockfd;
	bind = assign.bind;
	
	return (*this);
}

void
ConnInfo::reset()
{
	sockfd = Ws::FD_NONE;
	std::memset(&bind, 0, sizeof(bind));
}