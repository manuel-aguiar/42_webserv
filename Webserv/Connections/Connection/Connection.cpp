

#include "Connection.hpp"
#include "../InternalManager/InternalManager.hpp"
#include "../Monitor/Monitor.hpp"
#include <unistd.h>

namespace Conn
{
	Connection::Connection(InternalManager& connManager) :
		m_connManager(connManager)
	{
		m_monitor.acquire(accessEventManager());
	}

	Connection::~Connection()
	{
		m_monitor.release(accessEventManager());
	}

	void    
	Connection::close()
	{
		m_appLayer.close(m_socket);
		m_connManager._ReturnConnection(*this);
	}

	ServerContext&
	Connection::accessServerContext()
	{
		return (m_connManager._accessServerContext());
	}

	Events::Manager&
	Connection::accessEventManager()
	{
		return (m_connManager._accessEventManager());
	}

	void
	Connection::startMonitoring(bool isCalledFromEventLoop)
	{
		m_monitor.subscribe(accessEventManager(), isCalledFromEventLoop);
	}
	
	void
	Connection::stopMonitoring(bool isCalledFromEventLoop)
	{
		m_monitor.unsubscribe(accessEventManager(), isCalledFromEventLoop);
	}
	
	void
	Connection::updateMonitoring(bool isCalledFromEventLoop)
	{
		m_monitor.modify(accessEventManager(), isCalledFromEventLoop);
	}
	

}
