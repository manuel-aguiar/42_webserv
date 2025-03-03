

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
    std::cout << message << std::endl;
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


	/*
		Error or Hangup:
			- close the connection (we as a server have nothing left to do)

		Read:
			- read from the socket into the read buffer
			- if there is no request or the last request is completed, create a new request
			- parse the read buffer into the request

		Write: if there is data left in the write buffer, write it to the socket (until all data is written)
			- if there is no data left in the write buffer, check if there is a response to write
			- if there is a response to write, fill the write buffer with the response and write it to the socket
				- if not all can be written in one go, writeOffset != m_size, back to step 1
	*/





	/*
		Cleanup everything associated with it:
		- unsubscribe events
		- delete request and response queues
		- unsubscribe its timer at the Http::Module
		- return itself to the Http::Module
		- lastly, close the Conn::HttpConnection associated with it
	*/

	void
	Connection::setMyTCP(Conn::Connection& tcpConn)
	{
		m_tcpConn = &tcpConn;
		m_transaction.response.setConnectionAddress(tcpConn.info_getListenInfo().addr.sockaddr);
	}

	void
	Connection::setMyTimer(TimerTracker<Timer, Http::Connection*>::iterator timer)
	{
		m_myTimer = timer;
	}

	void
	Connection::closeConnection()
	{
		std::cout << "\t\t" <<  this << " closing connection" << std::endl;
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
		{
			std::cout << "\t\t" <<  this << " closing connection, tcpConn is already NULL" << std::endl;
			return ;
		}
		Conn::Connection* tcpConn = m_tcpConn;
		m_tcpConn = NULL;
		tcpConn->close();
	}

	void
	Connection::resetTransaction()
	{
		std::cout << "resetting transaction" << std::endl;
		m_transaction.reset();
		m_readBuffer.clear();
		m_writeBuffer.clear();
	}

}	// end of http namespace
