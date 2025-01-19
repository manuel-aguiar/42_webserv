

# include "Connection.hpp"
# include "../Event/Event.hpp"
# include "../Globals/LogFile/LogFile.hpp"
# include "../ServerManager/ListeningSocket/ListeningSocket.hpp"

Connection::Connection(Globals* globals) :
    m_globals(globals)
{

}

Connection::~Connection()
{

}

void    Connection::init()
{
    m_memPool = Nginx_MemoryPool::create(4096, 10);
}

void    Connection::reset()
{
    m_sockfd = -1;
    m_listener = NULL;
    m_memPool->reset();
}

void    Connection::read()
{

}


void    Connection::write()
{

}


// no copies, as usual
Connection::Connection(const Connection& other) { (void)other;}
Connection& Connection::operator=(const Connection& other) {(void)other; return (*this);}
