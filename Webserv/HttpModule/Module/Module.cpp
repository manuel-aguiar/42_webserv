


// project headers
# include "Module.hpp"
# include "../Connection/Connection.hpp"
# include "../../ServerConfig/DefaultConfig/DefaultConfig.hpp"

// C++ headers
# include <cstdlib>

namespace Http
{

Module::Module(const size_t maxConnections, const DefaultConfig& config, Globals& globals)
    : m_globals(globals),
      m_global_maxHeaderSize(std::atoi(config.maxClientHeaderSize.c_str())),
      m_global_timeouts(0),
      m_connections(maxConnections),
      m_availableConnections(maxConnections),
      m_timers(maxConnections)
{
    // setup global timeouts
    m_global_timeouts[Http::Timeout::FULL_REQUEST] = std::atoi(config.timeoutFullHeader.c_str());
    m_global_timeouts[Http::Timeout::INTER_SEND] = std::atoi(config.timeoutInterSend.c_str());
    m_global_timeouts[Http::Timeout::INTER_RECEIV] = std::atoi(config.timeoutInterReceive.c_str());

    // create and load connections
    for (size_t i = 0; i < maxConnections; ++i)
    {
        m_connections.emplace_back(*this);
        m_availableConnections.emplace_back(&m_connections.back());
    }

}

    Http::Connection*
    Module::acquireConnection()
    {
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

}	// end of namespace Http