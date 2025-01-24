

# include "Connection.hpp"

namespace Conn
{
	//getters
	t_socket				Manager::Connection::getSocket() const
	{
		return (m_sockfd);
	}

	const Ws::Sock::addr&		Manager::Connection::getAddr() const
	{
		return (m_addr);
	}

	const Events::Subscription&			Manager::Connection::getEventSubs() const
	{
		return (*m_eventSubs);
	}

	const Events::Subscription&			Manager::Connection::getWriteEvent() const
	{
		return (*m_writeEvent);
	}

	const ListeningSocket&	Manager::Connection::getListener() const
	{
		return (*m_listener);
	}

	const Globals&			Manager::Connection::getGlobals() const
	{
		return (m_globals);
	}

	//setters
	void					Manager::Connection::setSocket(const t_socket sockfd)
	{
		m_sockfd = sockfd;
	}

	void					Manager::Connection::setAddr(const Ws::Sock::addr& addr)
	{
		m_addr = addr;
	}

	void					Manager::Connection::setAddrlen(const t_socklen addrlen)
	{
		m_addrlen = addrlen;
	}


	void					Manager::Connection::setListener(ListeningSocket& listener)
	{
		m_listener = &listener;
	}

	void					Manager::Connection::setProtoConnection(const t_ptr_ProtoConnection connection)
	{
		m_ptr_protoConnection = connection;
	}

	void					Manager::Connection::setProtoModule(const t_ptr_ProtoModule module)
	{
		m_ptr_protoModule = module;
	}


	//accessors
	t_ptr_ProtoConnection	Manager::Connection::accessProtoConnection()
	{
		return (m_ptr_protoConnection);
	}

	t_ptr_ProtoModule		Manager::Connection::accessProtoModule()
	{
		return (m_ptr_protoModule);
	}

	Nginx_MemoryPool&		Manager::Connection::accessMemPool()
	{
		return (m_memPool);
	}

	t_sockaddr*		Manager::Connection::accessAddr()
	{
		return (m_addr);
	}
}
