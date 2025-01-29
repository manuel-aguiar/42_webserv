

# include "ListeningSocket.hpp"
# include "../InternalManager/InternalManager.hpp"

ListeningSocket::ListeningSocket(const int backlog, const Ws::BindInfo& info, InternalManager& connManager) :
	m_socket(-1, info),
	m_accepter(backlog),
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

Events::Manager&
ListeningSocket::mf_accessEventManager()
{
	return (m_connManager._accessEventManager());
}

