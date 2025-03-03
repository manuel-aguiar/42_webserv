

#include "Connection.hpp"
#include "../InternalManager/InternalManager.hpp"
#include "../Monitor/Monitor.hpp"
#include <unistd.h>

namespace Conn
{
	Connection::Connection(Events::Manager& eventManager, ServerContext& context, InternalManager* connManager) :
		m_monitor(eventManager),
		m_serverContext(context),
		m_connManager(connManager)
	{
		m_monitor.acquire();
	}

	Connection::~Connection()
	{
		m_monitor.release();
	}

	Connection::Connection(const Connection& copy) :
		m_monitor(copy.m_monitor),
		m_peerInfo(copy.m_peerInfo),
		m_serverContext(copy.m_serverContext),
		m_connManager(copy.m_connManager)
	{
		m_monitor.acquire();
	}

	Connection&
	Connection::operator=(const Connection& assign)
	{
		ASSERT_EQUAL(m_connManager, assign.m_connManager, "Connection::operator=(), copy assignment, copy-from InternalManager not the same as copy-to InternalManager");
		ASSERT_EQUAL(&m_serverContext, &assign.m_serverContext, "Connection::operator=(), copy assignment, copy-from connections from different servercontexts");
		if (this == &assign)
			return (*this);

		m_monitor = assign.m_monitor;
		m_connManager = assign.m_connManager;

		return (*this);
	}

	void    
	Connection::close()
	{
		ASSERT_EQUAL(m_connManager != NULL, true, "Connection::close(), connManager is NULL");
		//std::cout << "Connection::close()" << std::endl;
		m_monitor.unsubscribe(true);
		if (m_peerInfo.sockfd != Ws::FD_NONE)
		{
			::close(m_peerInfo.sockfd);
			m_peerInfo.sockfd = Ws::FD_NONE;
		}
		m_connManager->_ReturnConnection(*this);
	}

	ServerContext&
	Connection::accessServerContext()
	{
		return (m_serverContext);
	}

	Events::Manager&
	Connection::accessEventManager()
	{
		return (m_monitor.accessEventManager());
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
		return (m_appConn);
	}
	
	Ws::AppLayer::CloseCallback
	Connection::appLayer_accessCloseCallback()
	{
		return (m_appForceClose);
	}

	void
	Connection::appLayer_setConn			(const Ws::AppLayer::Conn& appConn)
	{
		m_appConn = appConn;
	}
	
	void
	Connection::appLayer_setCloseCallback	(const Ws::AppLayer::CloseCallback callback)
	{
		m_appForceClose = callback;
	}
}
