

# include "InternalConn.hpp"
# include "../../Events/Subscription/Subscription.hpp"

InternalConn::InternalConn(ImplManager& connManager) :
	Conn::Connection(connManager)
{
}

InternalConn::~InternalConn()
{
}

int
InternalConn::beAccepted(Ws::Sock::fd listener, Ws::Sock::type type, Ws::Sock::protocol proto, Ws::AppLayer::Type appLayer)
{
	m_sockfd = ::accept(listener, (struct sockaddr*)(&m_info.addr), &m_info.addrlen);

	if (m_sockfd == -1)
		return (m_sockfd);

	m_info.family = m_info.addr.sockaddr.sa_family;
	m_info.socktype = type;
	m_info.proto = proto;
	m_info.appLayer = appLayer;

	return (m_sockfd);
}

void
InternalConn::ForcedClose()
{
	if (m_sockfd == -1)
		return ;
	if (m_appForceClose)
		m_appForceClose(*this);
	Conn::Connection::close();
}

void
InternalConn::reset()
{
	Conn::Connection::pf_reset();
}

InternalConn::InternalConn(const InternalConn& copy) :
	Conn::Connection(copy)
{
}

InternalConn& InternalConn::operator=(const InternalConn& assign)
{
	if (this == &assign)
		return (*this);
	return (*this);
}