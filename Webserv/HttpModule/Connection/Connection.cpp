

# include "Connection.hpp"
# include "../Module/Module.hpp"
# include "../Request/Request.hpp"
# include "../Response/Response.hpp"
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
	  m_requests(),
	  m_responses() {}

Connection::~Connection() {}

void
Connection::ReadWrite_Callback(Events::Subscription& subscription)
{
	Http::Connection* connection = reinterpret_cast<Http::Connection*>(subscription.accessUser());
	connection->ReadWrite();
}


/*

	If the client closes the connection, there is nothing left to do:
	close all interactions with other modules (eg stop cgi), remove timers
	delete request/responses, close the Conn::Connection, return
	the Http::Connection back to the Http::Module for future use

*/

void
Connection::ReadWrite()
{
	int triggeredEvents = m_tcpConn->events_getTriggeredEvents();

	if (triggeredEvents & (Events::Monitor::ERROR | Events::Monitor::HANGUP))
	{
		close();
		return ;
	}
}



/*
	Cleanup everything associated with it:
	- unsubscribe events
	- delete request and response queues
	- unsubscribe its timer at the Http::Module
	- return itself to the Http::Module
	- lastly, close the Conn::Connection associated with it
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

	m_requests.clear();
	m_responses.clear();

	m_module.returnConnection(*this);
	m_tcpConn->close();
}
}	// end of http namespace