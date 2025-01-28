

# include "Connection.hpp"

//getters
namespace Conn
{
	Monitor&
	Connection::accessMonitor()
	{
		return (m_monitor);
	}

	//setters
	AppLayer&
	Connection::accessAppLayer()
	{
		return (m_appLayer);
	}

	Socket&
	Connection::accessSocket()
	{
		return (m_socket);
	}

}

