

#include "ListeningSocket.hpp"

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

const EventCallback&				ListeningSocket::getEvent()						const
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

EventCallback&						ListeningSocket::accessEvent()
{
	return (m_event);
}

