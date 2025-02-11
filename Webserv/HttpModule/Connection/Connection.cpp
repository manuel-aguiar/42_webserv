

# include "Connection.hpp"
# include "../Module/Module.hpp"
# include "../../Events/Subscription/Subscription.hpp"
# include "../../Connections/Connection/Connection.hpp"

namespace Http
{

void
Connection::ReadWrite_Callback(Events::Subscription& subscription)
{
	Http::Connection* connection = reinterpret_cast<Http::Connection*>(subscription.accessUser());
	connection->ReadWrite();

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