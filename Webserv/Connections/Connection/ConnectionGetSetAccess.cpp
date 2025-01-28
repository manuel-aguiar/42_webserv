

# include "Connection.hpp"

//getters
namespace Conn
{
	const Ws::BindInfo&		Connection::getBindInfo() const
	{
		return (m_info);
	}

	Ws::Sock::fd			Connection::getSocket() const
	{
		return (m_sockfd);
	}

	//setters
	void	Connection::setAppLayerConn			(const Ws::AppLayer::Conn& appConn)
	{
		m_appConn = appConn;
	}

	void 	Connection::setAppLayerCloseCallback	(const Ws::AppLayer::CloseCallback callback)
	{
		m_appForceClose = callback;
	}

	//accessors
	Ws::AppLayer::Conn	Connection::accessAppLayerConn()
	{
		return (m_appConn);
	}

	Ws::AppLayer::CloseCallback	Connection::accessAppLayerCloseCallback()
	{
		return (m_appForceClose);
	}
}

