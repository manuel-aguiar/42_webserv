



# include "Connection.hpp"
# include "../../ListeningSocket/ListeningSocket.hpp"
# include "../Events/Manager/Manager.hpp"

namespace Conn
{
    Connection::Connection(	Events::Manager& eventManager, 
						    ImplManager& connManager, 
						    ServerContext& serverContext) :
        m_sockfd(-1),
        m_connManager(connManager),
        m_eventManager(eventManager),
        m_context(serverContext)
    {
        m_eventSubs = m_eventManager.acquireSubscription();
    }

    Connection::~Connection()
    {
        
    }

    void    Connection::reset()
    {
        m_sockfd = -1;
    }


    // no copies, as usual
    Connection::Connection(const Connection& other) :
        m_sockfd(other.m_sockfd),
        m_addr(other.m_addr),
        m_addrlen(other.m_addrlen),
        m_eventSubs(other.m_eventSubs),
        m_connManager(other.m_connManager),
        m_eventManager(other.m_eventManager),
        m_context(other.m_context)
    {

    }

    Connection& Connection::operator=(const Connection& other) {(void)other; return (*this);}
}