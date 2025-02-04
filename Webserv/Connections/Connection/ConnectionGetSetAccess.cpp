

# include "Connection.hpp"

//getters
namespace Conn
{

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

	Events::Subscription&
	Connection::accessEvent()
	{
		return (m_monitor.accessEvent());
	}
}

