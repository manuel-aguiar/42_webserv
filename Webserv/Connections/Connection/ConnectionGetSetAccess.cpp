

# include "Connection.hpp"

//getters
namespace Conn
{
	Ws::Sock::fd
	Connection::info_getFd() const
	{
		return (m_info.sockfd);
	}

	const Ws::BindInfo&
	Connection::info_getBindInfo() const
	{
		return (m_info.bind);
	}
}

