

#include "Connection.hpp"
#include "../InternalManager/InternalManager.hpp"

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
		m_appLayer.close(m_socket);
		m_connManager._ReturnConnection(*this);
	}

	/*
		closes the connection if open.
		resets all parameters to base value for later reuse
	*/


	ServerContext&
	Connection::accessServerContext()
	{
		return (m_connManager._accessServerContext());
	}

}
