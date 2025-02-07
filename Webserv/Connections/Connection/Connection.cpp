

#include "Connection.hpp"
#include "../InternalManager/InternalManager.hpp"
#include "../Monitor/Monitor.hpp"
#include <unistd.h>

namespace Conn
{
	Connection::Connection(InternalManager& connManager) :
		m_monitor(connManager._accessEventManager()),
		m_connManager(connManager)
	{
		m_monitor.acquire();
	}

	Connection::~Connection()
	{
		m_monitor.release();
	}

	void    
	Connection::close()
	{
		//std::cout << "Connection::close()" << std::endl;
		m_monitor.unsubscribe(false);
		if (m_socket.getSockFd() != Ws::FD_NONE)
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
	Connection::events_startMonitoring(bool isCalledFromEventLoop)
	{
		m_monitor.subscribe(isCalledFromEventLoop);
	}
	
	void
	Connection::events_stopMonitoring(bool isCalledFromEventLoop)
	{
		m_monitor.unsubscribe(isCalledFromEventLoop);
	}
	
	void
	Connection::events_updateMonitoring(bool isCalledFromEventLoop)
	{
		m_monitor.modify(isCalledFromEventLoop);
	}


	Ws::AppLayer::Conn
	Connection::appLayer_accessConn()
	{
		return (m_appLayer.accessConn());
	}
	
	Ws::AppLayer::CloseCallback
	Connection::appLayer_accessCloseCallback()
	{
		return (m_appLayer.accessCloseCallback());
	}

	void
	Connection::appLayer_setConn			(const Ws::AppLayer::Conn& appConn)
	{
		m_appLayer.setConn(appConn);
	}
	
	void
	Connection::appLayer_setCloseCallback	(const Ws::AppLayer::CloseCallback callback)
	{
		m_appLayer.setCloseCallback(callback);
	}


}
