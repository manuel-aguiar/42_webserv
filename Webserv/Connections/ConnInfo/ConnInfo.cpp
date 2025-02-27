

#include "ConnInfo.hpp"
#include <cstring> // memset
#include <cstring> // memset

ConnInfo::ConnInfo(const Ws::Sock::fd sockfd, const Ws::BindInfo& info) : 
	sockfd(sockfd),
	peerInfo(info),
	listenInfo(NULL) {}

ConnInfo::ConnInfo() : 
	sockfd(Ws::FD_NONE),
	peerInfo(),
	listenInfo(NULL)
{
	std::memset(&peerInfo, 0, sizeof(peerInfo));
}

ConnInfo::~ConnInfo() {}

ConnInfo::ConnInfo(const ConnInfo& copy) : 
	sockfd(copy.sockfd),
	peerInfo(copy.peerInfo),
	listenInfo(copy.listenInfo) {}

ConnInfo& ConnInfo::operator=(const ConnInfo& assign)
{
	if (this == &assign)
		return (*this);

	sockfd = assign.sockfd;
	peerInfo = assign.peerInfo;
	listenInfo = assign.listenInfo;
	
	return (*this);
}

void
ConnInfo::reset()
{
	sockfd = Ws::FD_NONE;
	std::memset(&peerInfo, 0, sizeof(peerInfo));
	listenInfo = NULL;
}
