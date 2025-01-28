

#include "Connection.hpp"
#include "../ImplManager/ImplManager.hpp"
#include "../../Events/Manager/Manager.hpp"
#include "../../Events/Subscription/Subscription.hpp"

#include <unistd.h>

namespace Conn
{
	Connection::Connection(ImplManager& connManager) :
		m_sockfd(-1),
		m_info((Ws::BindInfo){}),
		m_appConn(NULL),
		m_appForceClose(NULL),
		m_eventSubs(NULL),
		m_connManager(connManager)
	{
		m_eventSubs = mf_accessEventManager().acquireSubscription();
		ASSERT_EQUAL(m_eventSubs != NULL, true, "Connection::Connection, acquireSubscription() failed, should have enough for all");
	}

	Connection::~Connection()
	{
		mf_accessEventManager().returnSubscription(*m_eventSubs);
	}

	void    
	Connection::close()
	{
		if (m_sockfd != -1)
			::close(m_sockfd);
		m_sockfd = -1;
		m_connManager._ReturnConnection(*this);
	}

	/*
		closes the connection if open.
		resets all parameters to base value for later reuse
	*/
	void    
	Connection::pf_reset()
	{
		m_info = (Ws::BindInfo){};
		m_appConn = NULL;
		m_appForceClose = NULL;
		m_eventSubs->reset();
	}

	Events::Manager&
	Connection::mf_accessEventManager()
	{
		return (m_connManager._accessEventManager());
	}

	Globals&
	Connection::mf_accessGlobals()
	{
		return (m_connManager._accessGlobals());
	}

	ServerContext&
	Connection::mf_accessServerContext()
	{
		return (m_connManager._accessServerContext());
	}

	void
	Connection::subscribeEvents(bool isCalledFromEventLoop)
	{
		mf_accessEventManager().startMonitoring(*m_eventSubs, isCalledFromEventLoop);
	}

	void
	Connection::unsubscribeEvents(bool isCalledFromEventLoop)
	{
		mf_accessEventManager().stopMonitoring(*m_eventSubs, isCalledFromEventLoop);
	}

	void
	Connection::modifyEvents(bool isCalledFromEventLoop)
	{
		mf_accessEventManager().updateEvents(*m_eventSubs, isCalledFromEventLoop);
	}

	Events::Subscription*
	Connection::accessEvent()
	{
		return (m_eventSubs);
	}

	ServerContext&
	Connection::accessServerContext()
	{
		return (mf_accessServerContext());
	}


	void  
	Connection::pf_callAppLayerForceClose()
	{
		if (m_appForceClose)
			m_appForceClose(*this);
	}

	//private anyways
	Connection::Connection(const Connection& other) :
		m_sockfd(other.m_sockfd),
		m_info(other.m_info),
		m_appConn(other.m_appConn),
		m_appForceClose(other.m_appForceClose),
		m_eventSubs(other.m_eventSubs),
		m_connManager(other.m_connManager)
	{

	}

	Connection&
	Connection::operator=(const Connection& other)
	{
		if (this == &other)
			return (*this);

		return (*this);
	}
}
