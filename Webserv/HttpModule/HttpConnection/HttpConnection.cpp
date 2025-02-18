

# include "HttpConnection.hpp"
# include "../HttpModule/HttpModule.hpp"
# include "../HttpTransaction/HttpTransaction.hpp"
# include "../HttpRequest/HttpRequest.hpp"
# include "../HttpResponse/HttpResponse.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Connections/Connection/Connection.hpp"

namespace Http
{

Connection::Connection(Http::Module& module)
	: m_module(module),
	  m_readTimer(),
	  m_writeTimer(),
	  m_myTimer(),
	  m_tcpConn(NULL),
	  m_transactions() {}

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

void
Connection::ReadWrite()
{
	ASSERT_EQUAL(m_tcpConn != NULL, true, "HttpConnection::ReadWrite(): m_tcpConn is NULL, event subscribed without owning a tcp connection");

	int triggeredEvents = m_tcpConn->events_getTriggeredEvents();
	int sockfd = m_tcpConn->info_getFd();

	// error
	if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
	{
		close();
		return ;
	}

	// read
	if (triggeredEvents & Events::Monitor::READ)
	{
		m_readBuffer.read(sockfd);
		if (m_transactions.size() == 0 || m_transactions.back().request.getParsingState() == Http::Request::COMPLETED)
			m_transactions.push_back(Transaction(*this, m_tcpConn->accessServerContext()));
		m_transactions.back().request.parse(m_readBuffer);
	}

	// write
	if (triggeredEvents & Events::Monitor::WRITE)
	{
		if (m_writeBuffer.writeOffset() != m_writeBuffer.size())
		{
			m_writeBuffer.write(sockfd, m_writeBuffer.writeOffset());
			return ;
		}
		if (m_transactions.size() == 0)
			return ;
		m_transactions.front().response.fillWriteBuffer(m_writeBuffer);
		m_writeBuffer.write(sockfd);
	}
}



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
}

void
Connection::close()
{
	m_tcpConn->events_stopMonitoring(true);

	if (m_myTimer != TimerTracker<Timer, Http::Connection*>::iterator())
		m_module.removeTimer(m_myTimer);
	
	// reset timer related stuff
	m_myTimer = TimerTracker<Timer, Http::Connection*>::iterator();
	m_readTimer = Timer();
	m_writeTimer = Timer();

	m_transactions.clear();

	m_module.returnConnection(*this);
	m_tcpConn->close();
}

Buffer&
Connection::accessReadBuffer()
{
	return (m_readBuffer);
}

Buffer&
Connection::accessWriteBuffer()
{
	return (m_writeBuffer);
}


}	// end of http namespace