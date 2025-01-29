

# include "InternalListen.hpp"
# include "../InternalManager/InternalManager.hpp"

InternalListen::InternalListen(const int backlog, const Ws::BindInfo& info, InternalManager& connManager) :
	m_socket(-1, info),
	m_listener(backlog),
	m_connManager(connManager)
{
	
}

InternalListen::~InternalListen()
{
	close();
}

Globals&
InternalListen::mf_accessGlobals()
{
	return (m_connManager._accessGlobals());
}

ServerContext&
InternalListen::mf_accessServerContext()
{
	return (m_connManager._accessServerContext());
}

Events::Manager&
InternalListen::mf_accessEventManager()
{
	return (m_connManager._accessEventManager());
}

