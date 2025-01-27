



# include "Connection.hpp"
# include "../..//ListeningSocket/ListeningSocket.hpp"


namespace Conn
{
    ImplManager::Connection::Connection(Globals& globals) :
        m_globals(globals)
    {

    }

    ImplManager::Connection::~Connection()
    {
        m_memPool.destroy();
    }

    void    ImplManager::Connection::init()
    {
        
    }

    void    ImplManager::Connection::reset()
    {
        m_sockfd = -1;
        m_listener = NULL;
    }


    // no copies, as usual
    ImplManager::Connection::Connection(const Connection& other) :
        m_sockfd(other.m_sockfd),
        m_addr(other.m_addr),
        m_addrlen(other.m_addrlen),
        m_eventSubs(other.m_eventSubs),
        m_listener(other.m_listener),
        m_globals(other.m_globals)
    {

    }

    ImplManager::Connection& ImplManager::Connection::operator=(const Connection& other) {(void)other; return (*this);}
}