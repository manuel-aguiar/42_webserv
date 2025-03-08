

# include "HttpConnection.hpp"
# include "../HttpModule/HttpModule.hpp"
# include "../HttpTransaction/HttpTransaction.hpp"
# include "../HttpRequest/HttpRequest.hpp"
# include "../HttpResponse/HttpResponse.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Connections/Connection/Connection.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"
# include "../../ServerContext/ServerContext.hpp"

# include <arpa/inet.h>

void    logDisconnection(Globals& globals, Conn::Connection& connection)
{
    const Ws::BindInfo& peer = connection.info_getPeerInfo();
    const Ws::BindInfo& listen = connection.info_getListenInfo();
    char ipPeer[INET_ADDRSTRLEN];
    char ipListen[INET_ADDRSTRLEN];

    ::inet_ntop(AF_INET, &peer.addr.sockaddr_in.sin_addr, ipPeer, INET_ADDRSTRLEN);
    int portPeer = ::ntohs(peer.addr.sockaddr_in.sin_port);

    ::inet_ntop(AF_INET, &listen.addr.sockaddr_in.sin_addr, ipListen, INET_ADDRSTRLEN);
    int portListen = ::ntohs(listen.addr.sockaddr_in.sin_port);

    std::string message = "Client disconnected from " + std::string(ipPeer) + ":" 
                            + StringUtils::to_string(portPeer) 
                            + " on listener " 
                            + std::string(ipListen) + ":" 
                            + StringUtils::to_string(portListen);
    globals.logStatus(message);
    //std::cout << message << std::endl;
}

namespace Http
{

	Connection::Connection(Http::Module& module, const size_t readBufferSize, const size_t writeBufferSize)
		: m_module(module)
		, m_liveTimeout(Timeout::NONE)
		, m_myTimer()
		, m_tcpConn(NULL)
		, m_transaction(m_module.accessServerContext())
		, m_readBuffer (readBufferSize) // 4kb, could be configurable, no effect on performance
		, m_writeBuffer (writeBufferSize) // 4kb, could be configurable, no effect on performance
		{}

	Connection::~Connection() {}

	void
	Connection::ReadWrite_Callback(Events::Subscription& subscription)
	{
		Http::Connection* connection = reinterpret_cast<Http::Connection*>(subscription.accessUser());
		connection->ReadWrite();
	}

	void
	Connection::prepareConnection(Conn::Connection& tcpConn)
	{
		Ws::fd sockfd = tcpConn.info_getFd();

		m_tcpConn = &tcpConn;
		m_transaction.response.setListenAddress(tcpConn.info_getListenInfo().addr.sockaddr);
		m_transaction.response.setTcpConnection(tcpConn);
		m_transaction.request.setBuffer_ReadFd(m_readBuffer, sockfd);
		m_transaction.response.setBuffer_writeFd(m_writeBuffer, sockfd);
	}

	void
	Connection::setMyTimer(TimerTracker<Timer, Http::Connection*>::iterator timer,
						const Http::Connection::Timeout::Type timeoutType)
	{
		m_myTimer = timer;
		m_liveTimeout = timeoutType;
		
		// cosy debug print

		//std::cout << this << " connection, update timer ";
		//switch (timeoutType)
		//{
		//	case Timeout::KEEP_ALIVE:
		//		std::cout << "KEEP_ALIVE";
		//		break ;
		//	case Timeout::FULL_HEADER:
		//		std::cout << "FULL_HEADER";
		//		break ;
		//	case Timeout::INTER_SEND:
		//		std::cout << "INTER_SEND";
		//		break ;
		//	case Timeout::INTER_RECV:
		//		std::cout << "INTER_RECV";
		//		break ;
		//	default:
		//		std::cout << "NONE";
		//		break ;
		//}
		//std::cout << " set to " << (timer->first - Timer::now()) << std::endl;
	}

	Http::Connection::Timeout::Type
	Connection::getLiveTimeoutType() const
	{
		return (m_liveTimeout);
	}

	void
	Connection::closeConnection()
	{
		logDisconnection(*m_tcpConn->accessServerContext().getGlobals(), *m_tcpConn);
		m_tcpConn->events_stopMonitoring(true);

		if (m_myTimer != TimerTracker<Timer, Http::Connection*>::iterator())
			m_module.removeTimer(m_myTimer);
		
		// reset timer related stuff
		m_myTimer = TimerTracker<Timer, Http::Connection*>::iterator();
		m_liveTimeout = Timeout::NONE;
		resetTransaction();
		m_transaction.response.close();

		m_module.returnConnection(*this);

		if (m_tcpConn == NULL)
			return ;
		Conn::Connection* tcpConn = m_tcpConn;
		m_tcpConn = NULL;
		tcpConn->close();
	}

	void
	Connection::resetTransaction()
	{
		m_transaction.reset();
		m_readBuffer.clear();
		m_writeBuffer.clear();
	}

	void
	Connection::closeTransaction()
	{
		m_transaction.close();
		m_readBuffer.clear();
		m_writeBuffer.clear();
	}

}	// end of http namespace
