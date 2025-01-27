
# include "InternalConn.hpp"

InternalConn::InternalConn(	Events::Manager& eventManager, 
							ImplManager& connManager, 
							ServerContext& serverContext) :
	Conn::Connection(eventManager, connManager, serverContext) {}

InternalConn::~InternalConn() {}

InternalConn::InternalConn(const InternalConn& other) :
	Conn::Connection(other) {}

void	InternalConn::ForceClose()
{
	if (m_state == ACTIVE)
		Conn::Connection::CallUserForceClose();
	Conn::Connection::reset();
	m_state == IDLE;
}

int	InternalConn::accept(Ws::Sock::fd& listener)
{
	m_sockfd = ::accept(listener, (struct sockaddr*)(&m_addr), &m_addrlen);
	return (m_sockfd);
}

InternalConn& InternalConn::operator=(const InternalConn& other)
{
	if (this == &other)
		return (*this);
	
	Conn::Connection::operator=(other);
	
	return (*this);
}