

# include "../Connection.hpp"
# include "../../EventCallback/EventCallback.hpp"
# include "../../Globals/LogFile/LogFile.hpp"
# include "../../ServerManager/ListeningSocket/ListeningSocket.hpp"

Connection::Connection(Globals& globals) :
    m_memPool(*(Nginx_MemoryPool *)NULL),
    m_globals(globals)
{
    (void)globals;
}

Connection::~Connection()
{
}

void    Connection::init()
{
    
}

void    Connection::reset()
{

}

void    Connection::read()
{

}


void    Connection::write()
{

}


// no copies, as usual
Connection::Connection(const Connection& other) :
    m_memPool(*(Nginx_MemoryPool *)NULL),
    m_globals(*(Globals *)NULL)
{
    (void)other;
}

Connection& Connection::operator=(const Connection& other) {(void)other; return (*this);}


//getters
t_socket				Connection::getSocket() const
{
	return (m_sockfd);
}

const t_sockaddr*		Connection::getAddr() const
{
	return (m_addr);
}

t_socklen				Connection::getAddrlen() const
{
	return (m_addrlen);
}

const EventCallback&			Connection::getReadEvent() const
{
	return (*m_readEvent);
}

const EventCallback&			Connection::getWriteEvent() const
{
	return (*m_writeEvent);
}

const ListeningSocket&	Connection::getListener() const
{
	return (*m_listener);
}

const Globals&			Connection::getGlobals() const
{
	return (m_globals);
}

const Nginx_MemoryPool&	Connection::getMemPool() const
{
	return (m_memPool);
}


//setters
void					Connection::setSocket(const t_socket sockfd)
{
	m_sockfd = sockfd;
}

void					Connection::setAddr(t_sockaddr* addr)
{
	m_addr = addr;
}

void					Connection::setAddrlen(const t_socklen addrlen)
{
	m_addrlen = addrlen;
}

void					Connection::setReadEvent(EventCallback& event)
{
	m_readEvent = &event;
}

void					Connection::setWriteEvent(EventCallback& event)
{
	m_writeEvent = &event;
}

void					Connection::setListener(ListeningSocket& listener)
{
	m_listener = &listener;
}

void					Connection::setProtoConnection(const t_ptr_ProtoConnection connection)
{
	m_ptr_protoConnection = connection;
}

void					Connection::setProtoModule(const t_ptr_ProtoModule module)
{
	m_ptr_protoModule = module;
}


//accessors
t_ptr_ProtoConnection	Connection::accessProtoConnection()
{
	return (m_ptr_protoConnection);
}

t_ptr_ProtoModule		Connection::accessProtoModule()
{
	return (m_ptr_protoModule);
}

Nginx_MemoryPool&		Connection::accessMemPool()
{
	return (m_memPool);
}

t_sockaddr*		Connection::accessAddr()
{
	return (m_addr);
}