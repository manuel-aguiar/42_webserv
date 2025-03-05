


// project headers
# include "HttpModule.hpp"
# include "../HttpConnection/HttpConnection.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../ServerConfig/DefaultConfig/DefaultConfig.hpp"
# include "../../TimerTracker/TimerTracker.hpp"
# include "../../TimerTracker/Timer/Timer.hpp"

// C++ headers
# include <cstdlib>

namespace Http
{

Module::Module(const size_t maxConnections, ServerContext& context)
    :   m_context(context),
        m_cgiInterface(*reinterpret_cast<Cgi::Module*>(context.getAddonLayer(Ws::AddonLayer::CGI)), maxConnections),
        m_connections(maxConnections),
        m_availableConnections(maxConnections),
        m_timers(maxConnections)
{
    // create and load connections
    for (size_t i = 0; i < maxConnections; ++i)
    {
        m_connections.emplace_back(*this);
        m_availableConnections.emplace_back(&m_connections.back());
    }

}

Module::~Module()
{
}

Http::Connection*
Module::acquireConnection()
{
    ASSERT_EQUAL(m_availableConnections.isEmpty(), false, "Http::Module::acquireConnection - there should be connections available for everyone");
    if (m_availableConnections.isEmpty())
        return (NULL);
    Http::Connection* connection = m_availableConnections.front();
    m_availableConnections.pop_front();
    return (connection);
}

void
Module::returnConnection(Http::Connection& connection)
{
    m_availableConnections.push_back(&connection);
}

TimerTracker<Timer, Http::Connection*>::iterator
Module::insertTimer(const Timer& timer, Http::Connection& connection)
{
    return (m_timers.insert(timer, &connection));
}

void
Module::removeTimer(const TimerTracker<Timer, Http::Connection*>::iterator position)
{
    m_timers.erase(position);
}

ServerContext&
Module::accessServerContext()
{
    return (m_context);
}

Http::CgiInterface&
Module::accessCgiInterface()
{
    return (m_cgiInterface);
}

int 
Module::closeTimedOutConnections()
{
    Timer timer = Timer::now();

    TimerTracker<Timer, Http::Connection*>::iterator 	it = m_timers.begin();
    Http::Connection* 								    curConnection;
    
    for (; it != m_timers.end(); ++it) 
    {
        if (it->first < timer)
        {
            curConnection = it->second;
            std::cout << "connection " << curConnection << " timed out" << std::endl;
            curConnection->closeConnection();
        }
        else
            break ;
    }
                    
    if (m_timers.begin() == m_timers.end())
        return (-1);
    return ((timer < m_timers.begin()->first) ? 1 : (m_timers.begin()->first - timer).getMilliseconds());        
}

}	// end of namespace Http
