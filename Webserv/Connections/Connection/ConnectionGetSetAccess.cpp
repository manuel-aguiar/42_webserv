

# include "Connection.hpp"

namespace Conn
{
	//getters
	t_socket				ImplManager::Connection::getSocket() const
	{
		return (m_sockfd);
	}

	const Ws::Sock::addr&		ImplManager::Connection::getAddr() const
	{
		return (m_addr);
	}

	const Events::Subscription&			ImplManager::Connection::getEventSubs() const
	{
		return (*m_eventSubs);
	}

	const Events::Subscription&			ImplManager::Connection::getWriteEvent() const
	{
		return (*m_writeEvent);
	}

	const ListeningSocket&	ImplManager::Connection::getListener() const
	{
		return (*m_listener);
	}

	const Globals&			ImplManager::Connection::getGlobals() const
	{
		return (m_globals);
	}

	//setters
	void					ImplManager::Connection::setSocket(const t_socket sockfd)
	{
		m_sockfd = sockfd;
	}

	void					ImplManager::Connection::setAddr(const Ws::Sock::addr& addr)
	{
		m_addr = addr;
	}

	void					ImplManager::Connection::setAddrlen(const t_socklen addrlen)
	{
		m_addrlen = addrlen;
	}


	void					ImplManager::Connection::setListener(ListeningSocket& listener)
	{
		m_listener = &listener;
	}

	void					ImplManager::Connection::setProtoConnection(const t_ptr_ProtoConnection connection)
	{
		m_ptr_protoConnection = connection;
	}

	void					ImplManager::Connection::setProtoModule(const t_ptr_ProtoModule module)
	{
		m_ptr_protoModule = module;
	}


	//accessors
	t_ptr_ProtoConnection	ImplManager::Connection::accessProtoConnection()
	{
		return (m_ptr_protoConnection);
	}

	t_ptr_ProtoModule		ImplManager::Connection::accessProtoModule()
	{
		return (m_ptr_protoModule);
	}

	Nginx_MemoryPool&		ImplManager::Connection::accessMemPool()
	{
		return (m_memPool);
	}

	t_sockaddr*		ImplManager::Connection::accessAddr()
	{
		return (m_addr);
	}
}
