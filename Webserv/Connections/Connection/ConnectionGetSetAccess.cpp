

# include "Connection.hpp"

//getters
namespace Conn
{

	ConnInfo&
	Connection::mf_connInfo()
	{
		return (m_info);
	}

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

