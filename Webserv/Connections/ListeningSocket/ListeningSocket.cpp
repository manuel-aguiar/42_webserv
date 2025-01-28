

# include "ListeningSocket.hpp"
# include "../ImplManager/ImplManager.hpp"

ListeningSocket::ListeningSocket(const int backlog, const Ws::BindInfo& info, ImplManager& connManager) :
	m_socket(-1, info),
	m_monitor(connManager._accessEventManager()),
	m_listener(backlog),
	m_connManager(connManager)
{
	
}

ListeningSocket::~ListeningSocket()
{
	close();
}

Globals&
ListeningSocket::mf_accessGlobals()
{
	return (m_connManager._accessGlobals());
}

ServerContext&
ListeningSocket::mf_accessServerContext()
{
	return (m_connManager._accessServerContext());
}


/*
	Accepts a socket conenction on the ListeningSocket. Will be called by the Manager
	via the EventCallbackAccept function pointer, and by the ServerWorker when a Connection instance
	is returned to try and recycle. 
*/

