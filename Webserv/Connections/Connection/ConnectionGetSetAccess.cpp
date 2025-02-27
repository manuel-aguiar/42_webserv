

# include "Connection.hpp"
# include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

//getters
namespace Conn
{
	Ws::Sock::fd
	Connection::info_getFd() const
	{
		return (m_peerInfo.sockfd);
	}

	const Ws::BindInfo&
	Connection::info_getPeerInfo() const
	{
		return (m_peerInfo.peerInfo);
	}

	const Ws::BindInfo&
	Connection::info_getListenInfo() const
	{
		ASSERT_EQUAL(m_peerInfo.listenInfo != NULL, true, "Connection::info_getListenInfo(), no listener is associated with this connection");
		return (*m_peerInfo.listenInfo);
	}
}
