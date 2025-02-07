

# include "InternalListener.hpp"
# include "../InternalManager/InternalManager.hpp"

InternalListener::InternalListener(const Ws::BindInfo& info, InternalManager& connManager) :
	m_listener(info),
	m_monitor(connManager._accessEventManager()),
	m_connManager(connManager) {}

InternalListener::InternalListener(const InternalListener& copy) :
	m_listener(copy.m_listener),
	m_monitor(copy.m_monitor),
	m_connManager(copy.m_connManager) {}



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

// private
InternalListener& InternalListener::operator=(const InternalListener& assign)
{	
	if (this == &assign)
		return (*this);

	ASSERT_EQUAL(&m_connManager, &assign.m_connManager, "InternalListener::operator=(), copy assignment, copy-from InternalManager not the same as copy-to InternalManager");

	m_listener = assign.m_listener;
	m_monitor = assign.m_monitor;

	return (*this);
}
