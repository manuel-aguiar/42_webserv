



# include "Connection.hpp"
# include "../..//ListeningSocket/ListeningSocket.hpp"


namespace Conn
{
    Manager::Connection::Connection(Globals& globals) :
        m_globals(globals)
    {

    }

    Manager::Connection::~Connection()
    {
        m_memPool.destroy();
    }

    void    Manager::Connection::init()
    {
        
    }

    void    Manager::Connection::reset()
    {
        m_sockfd = -1;
        m_listener = NULL;
    }


    // no copies, as usual
    Manager::Connection::Connection(const Connection& other) :
        m_sockfd(other.m_sockfd),
        m_addr(other.m_addr),
        m_addrlen(other.m_addrlen),
        m_eventSubs(other.m_eventSubs),
        m_listener(other.m_listener),
        m_globals(other.m_globals)
    {

    }

    Manager::Connection& Manager::Connection::operator=(const Connection& other) {(void)other; return (*this);}
}