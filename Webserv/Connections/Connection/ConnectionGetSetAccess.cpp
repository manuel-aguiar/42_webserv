

# include "Connection.hpp"

namespace Conn
{

	const Ws::Sock::addr&		Connection::getAddr() const
	{
		return (m_addr);
	}

	Events::Subscription*			Connection::accessEventSubs()
	{
		return (m_eventSubs);
	}

	ServerContext&			Connection::accessServerContext()
	{
		return (m_context);
	}

	//setters
	void					Connection::setSocket(const t_socket sockfd)
	{
		m_sockfd = sockfd;
	}

	void					Connection::setAddr(const Ws::Sock::addr& addr)
	{
		m_addr = addr;
	}

	void					Connection::setAddrlen(const t_socklen addrlen)
	{
		m_addrlen = addrlen;
	}

}
