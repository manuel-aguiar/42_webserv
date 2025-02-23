

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
	  m_transaction(*this) {}

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
	m_transaction.response.setConnectionAddress(tcpConn.info_getBindInfo().addr.sockaddr);
}

void
Connection::closeConnection()
{
	m_tcpConn->events_stopMonitoring(true);

	if (m_myTimer != TimerTracker<Timer, Http::Connection*>::iterator())
		m_module.removeTimer(m_myTimer);
	
	// reset timer related stuff
	m_myTimer = TimerTracker<Timer, Http::Connection*>::iterator();
	m_readTimer = Timer();
	m_writeTimer = Timer();

	resetTransaction();

	m_module.returnConnection(*this);

	if (m_tcpConn == NULL)
		return ;

	m_tcpConn->close();
	m_tcpConn = NULL;

}

void
Connection::resetTransaction()
{
	m_transaction.request.reset();
	m_transaction.response.reset();
	m_readBuffer.clear();
	m_writeBuffer.clear();
}

BaseBuffer&
Connection::accessReadBuffer()
{
	return (m_readBuffer);
}

BaseBuffer&
Connection::accessWriteBuffer()
{
	return (m_writeBuffer);
}


}	// end of http namespace