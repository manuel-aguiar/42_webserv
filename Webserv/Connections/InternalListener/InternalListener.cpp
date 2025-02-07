

# include "InternalListener.hpp"
# include "../InternalManager/InternalManager.hpp"

InternalListener::InternalListener(const Ws::BindInfo& info, InternalManager& connManager) :
	m_monitor(connManager._accessEventManager()),
	m_listener(info),
	m_connManager(connManager)
{
	
}

InternalListener::~InternalListener()
{
	close();
}

Globals&
InternalListener::mf_accessGlobals()
{
	return (m_connManager._accessGlobals());
}

ServerContext&
InternalListener::mf_accessServerContext()
{
	return (m_connManager._accessServerContext());
}

Events::Manager&
InternalListener::mf_accessEventManager()
{
	return (m_connManager._accessEventManager());
}

