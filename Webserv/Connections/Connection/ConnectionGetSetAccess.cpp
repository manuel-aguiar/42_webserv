

# include "Connection.hpp"

//getters
namespace Conn
{

	//setters
	AppLayer&
	Connection::mf_accessAppLayer()
	{
		return (m_appLayer);
	}

	Socket&
	Connection::mf_accessSocket()
	{
		return (m_socket);
	}

}

