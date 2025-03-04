

# include "HttpConnection.hpp"
# include "../HttpModule/HttpModule.hpp"
# include "../HttpTransaction/HttpTransaction.hpp"
# include "../HttpRequest/HttpRequest.hpp"
# include "../HttpResponse/HttpResponse.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Connections/Connection/Connection.hpp"
# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../ServerConfig/ServerBlock/ServerBlock.hpp"

# include "../../GenericUtils/StringUtils/StringUtils.hpp"

namespace Http
{

int
Connection::mf_readIntoReadBuffer(const Ws::fd fd)
{
    const ServerConfig& config = *reinterpret_cast<const ServerConfig*>(m_module.accessServerContext().getServerConfig());

    if (m_transaction.request.isCompleted() || m_transaction.request.isError())
        return (1);

    int readBytes = m_readBuffer.read(fd, m_readBuffer.size() == m_readBuffer.capacity() ? 0 : m_readBuffer.size());
    
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

    mf_parseReadBuffer();

    return (readBytes);
}

void
Connection::mf_parseReadBuffer()
{
    m_readBuffer.truncatePush(m_transaction.request.parse(m_readBuffer));
}

void
Connection::mf_newRequestSameConnection()
{
    const ServerBlock& serverBlock = *m_transaction.response.getResponseData().serverBlock;

    resetTransaction();

    m_module.removeTimer(m_myTimer);
    
    if (m_readBuffer.size() != 0)
    {
        setMyTimer(m_module.insertTimer(Timer::now() + serverBlock.getTimeoutFullHeader(), *this), Timeout::FULL_HEADER);
        return (mf_parseReadBuffer());
    }
    setMyTimer(m_module.insertTimer(Timer::now() + serverBlock.getTimeoutKeepAlive(), *this), Timeout::KEEP_ALIVE);
}

void
Connection::ReadWrite()
{
    ASSERT_EQUAL(m_tcpConn != NULL, true, std::string("HttpConnection::ReadWrite(): m_tcpConn is NULL, ") + StringUtils::to_string(this));

    Events::Monitor::Mask triggeredEvents = m_tcpConn->events_getTriggeredEvents();
    Ws::fd sockfd = m_tcpConn->info_getFd();

    if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
    {
        closeConnection();
        return ;
    }

    if (triggeredEvents & Events::Monitor::READ)
    {
        if (!mf_readIntoReadBuffer(sockfd))
        {
            closeConnection();
            return ;
        }
    }

    // write
    if (!(triggeredEvents & Events::Monitor::WRITE))
        return ;

    // still have data on buffer, write that first
    if (m_writeBuffer.writeOffset() != m_writeBuffer.size())
    {
        m_writeBuffer.write(sockfd, m_writeBuffer.writeOffset());

        m_module.removeTimer(m_myTimer);
        setMyTimer(m_module.insertTimer(Timer::now() 
        + m_transaction.response.getResponseData().serverBlock->getTimeoutInterSend(), *this), Timeout::INTER_SEND);

        if (m_writeBuffer.size() != 0)
            return ;

        if (m_transaction.response.getStatus() == Http::ResponseStatus::FINISHED)
            return (mf_newRequestSameConnection());
        else if (m_transaction.response.getStatus() == Http::ResponseStatus::MARK_TO_CLOSE)
            closeConnection(); // transaction finished
        
        return ;
    }

    switch (m_transaction.response.fillWriteBuffer(m_writeBuffer))
    {
        case Http::ResponseStatus::FINISHED:
        {	
            m_writeBuffer.write(sockfd);

            if (m_writeBuffer.size() == 0)
                return (mf_newRequestSameConnection());
            return ;
        }
        case Http::ResponseStatus::MARK_TO_CLOSE:
        {
            m_writeBuffer.write(sockfd);
            if (m_writeBuffer.size() == 0)
                closeConnection();
            return ;
        }
        case Http::ResponseStatus::WRITING:
        {
            m_writeBuffer.write(sockfd); // got data, write
            m_module.removeTimer(m_myTimer);
            setMyTimer(m_module.insertTimer(Timer::now() 
            + m_transaction.response.getResponseData().serverBlock->getTimeoutInterSend(), *this), Timeout::INTER_SEND);
            return ;
        }

        case Http::ResponseStatus::WAITING: // nothing
        {
            break ;
        }
    }
}
}
