

# include "HttpConnection.hpp"
# include "../HttpModule/HttpModule.hpp"
# include "../HttpTransaction/HttpTransaction.hpp"
# include "../HttpRequest/HttpRequest.hpp"
# include "../HttpResponse/HttpResponse.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Connections/Connection/Connection.hpp"
# include "../../ServerContext/ServerContext.hpp"
# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

namespace Http
{

Http::IOStatus::Type
Connection::mf_readEventHandler()
{
    const ServerConfig& config = *reinterpret_cast<const ServerConfig*>(m_module.accessServerContext().getServerConfig());

    Http::IOStatus::Type ioStatus = m_transaction.request.read();
    
    if (ioStatus == Http::IOStatus::MARK_TO_CLOSE)
        return (ioStatus);

    if (m_liveTimeout == Timeout::KEEP_ALIVE)
    {
        m_module.removeTimer(m_myTimer);
        setMyTimer(m_module.insertTimer(Timer::now() + config.getTimeoutFullHeader(), *this), Timeout::FULL_HEADER);
    }

    if (m_transaction.request.getParsingState() == Http::Request::BODY)
    {
        m_module.removeTimer(m_myTimer);
        setMyTimer(m_module.insertTimer(Timer::now() + config.getTimeoutInterReceive(), *this), Timeout::INTER_RECV);
    }

    return (mf_parseReadBuffer());
}

Http::IOStatus::Type
Connection::mf_parseReadBuffer()
{
    return (m_transaction.request.parse());
}

Http::IOStatus::Type
Connection::mf_newRequestSameConnection()
{
    const ServerConfig& config = *reinterpret_cast<const ServerConfig*>(m_module.accessServerContext().getServerConfig());
    const ServerBlock* serverBlock = m_transaction.response.getResponseData().serverBlock;

    resetTransaction();

    m_module.removeTimer(m_myTimer);
    
    if (m_readBuffer.size() != 0)
    {
        int timeoutFullHeader = (serverBlock != NULL) ? serverBlock->getTimeoutFullHeader() : config.getTimeoutFullHeader();
        setMyTimer(m_module.insertTimer(Timer::now() + timeoutFullHeader, *this), Timeout::FULL_HEADER);
        return (mf_parseReadBuffer());
    }

    int timeoutKeepAlive = (serverBlock != NULL) ? serverBlock->getTimeoutKeepAlive() : config.getTimeoutKeepAlive();
    setMyTimer(m_module.insertTimer(Timer::now() + timeoutKeepAlive, *this), Timeout::KEEP_ALIVE);

    return (Http::IOStatus::WAITING);
}

void
Connection::ReadWrite()
{
    ASSERT_EQUAL(m_tcpConn != NULL, true, std::string("HttpConnection::ReadWrite(): m_tcpConn is NULL, ") + StringUtils::to_string(this));
    const ServerConfig& config = *reinterpret_cast<const ServerConfig*>(m_module.accessServerContext().getServerConfig());

    Events::Monitor::Mask triggeredEvents = m_tcpConn->events_getTriggeredEvents();

    if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
    {
        closeConnection();
        return ;
    }

    if (triggeredEvents & Events::Monitor::READ)
    {
        if (mf_readEventHandler() == Http::IOStatus::MARK_TO_CLOSE)
        {
            closeConnection();
            return ;
        }
    }

    // write
    if (!(triggeredEvents & Events::Monitor::WRITE))
        return ;


    Http::IOStatus::Type ioStatus = m_transaction.response.write();

    switch (ioStatus)
    {
        case Http::IOStatus::FINISHED:
        {
            mf_newRequestSameConnection();
            return ;
        }
        case Http::IOStatus::MARK_TO_CLOSE:
        {
            closeConnection();
            return ;
        }
        case Http::IOStatus::WRITING:
        {
            m_module.removeTimer(m_myTimer);
            int timeOutInterSend = (m_transaction.response.getResponseData().serverBlock != NULL)
            ? m_transaction.response.getResponseData().serverBlock->getTimeoutInterSend() : config.getTimeoutInterSend();
            setMyTimer(m_module.insertTimer(Timer::now() + timeOutInterSend, *this), Timeout::INTER_SEND);
            return ;
        }
        default:
            break ;
    }

}
}
