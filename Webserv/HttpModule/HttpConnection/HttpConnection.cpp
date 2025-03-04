

# include "HttpConnection.hpp"
# include "../HttpModule/HttpModule.hpp"
# include "../HttpTransaction/HttpTransaction.hpp"
# include "../HttpRequest/HttpRequest.hpp"
# include "../HttpResponse/HttpResponse.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Connections/Connection/Connection.hpp"
# include "../../Globals/Globals.hpp"
# include "../../GenericUtils/StringUtils/StringUtils.hpp"

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

	Connection::Connection(Http::Module& module)
		: m_module(module)
		, m_readTimer()
		, m_writeTimer()
		, m_myTimer()
		, m_tcpConn(NULL)
		, m_transaction(m_module.accessServerContext()) {}

	Connection::~Connection() {}

	void
	Connection::ReadWrite_Callback(Events::Subscription& subscription)
	{
		Http::Connection* connection = reinterpret_cast<Http::Connection*>(subscription.accessUser());
		connection->ReadWrite();
	}

	void
	Connection::setMyTCP(Conn::Connection& tcpConn)
	{
		m_tcpConn = &tcpConn;
		m_transaction.response.setListenAddress(tcpConn.info_getListenInfo().addr.sockaddr);
		m_transaction.response.setTcpConnection(tcpConn);
	}

	void
	Connection::setMyTimer(TimerTracker<Timer, Http::Connection*>::iterator timer)
	{
		m_myTimer = timer;
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
		m_readTimer = Timer();
		m_writeTimer = Timer();

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

}	// end of http namespace
