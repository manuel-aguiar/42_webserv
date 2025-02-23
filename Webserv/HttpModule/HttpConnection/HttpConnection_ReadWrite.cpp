

# include "HttpConnection.hpp"
# include "../HttpModule/HttpModule.hpp"
# include "../HttpTransaction/HttpTransaction.hpp"
# include "../HttpRequest/HttpRequest.hpp"
# include "../HttpResponse/HttpResponse.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Connections/Connection/Connection.hpp"

namespace Http
{

void
Connection::mf_read(const Ws::fd fd)
{
    if (m_readBuffer.available() == m_readBuffer.capacity())
    {
        m_readBuffer.read(fd);
        m_transaction.request.parse(m_readBuffer);
    }

    //BufferView view(m_readBuffer.data(), m_readBuffer.size());
    //std::cout << "Read: " << view << std::endl;
}


void
Connection::ReadWrite()
{
    ASSERT_EQUAL(m_tcpConn != NULL, true, "HttpConnection::ReadWrite(): m_tcpConn is NULL, event subscribed without owning a tcp connection");

    Events::Monitor::Mask triggeredEvents = m_tcpConn->events_getTriggeredEvents();
    Ws::fd sockfd = m_tcpConn->info_getFd();

    if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
    {
        closeConnection();
        return ;
    }

    if (triggeredEvents & Events::Monitor::READ)
        mf_read(sockfd);

    // write
    if (!(triggeredEvents & Events::Monitor::WRITE))
        return ;

    // still have data on buffer, write that first
    if (m_writeBuffer.writeOffset() != m_writeBuffer.size())
    {
        m_writeBuffer.write(sockfd, m_writeBuffer.writeOffset());
        
        if (m_writeBuffer.size() != 0)
            return ;

        if (m_transaction.response.getStatus() == Http::ResponseStatus::FINISHED)
            resetTransaction();
        else if (m_transaction.response.getStatus() == Http::ResponseStatus::MARK_TO_CLOSE)
            closeConnection(); // transaction finished
        
        return ;
    }
    
    // ask response for data
    switch (m_transaction.response.fillWriteBuffer(m_writeBuffer))
    {
        case Http::ResponseStatus::FINISHED:
        {	
            m_writeBuffer.write(sockfd);

            if (m_writeBuffer.size() == 0)
                resetTransaction();
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
            return ;
        }

        case Http::ResponseStatus::WAITING: // nothing
            break ;
    }
}
}
