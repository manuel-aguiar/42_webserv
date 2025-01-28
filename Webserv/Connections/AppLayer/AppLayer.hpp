

#ifndef ACCEPTED_HPP

# define ACCEPTED_HPP

# include "../Socket/Socket.hpp"

class AppLayer
{
	public:
		AppLayer();
		~AppLayer();
		AppLayer(const AppLayer& copy);
		AppLayer& operator=(const AppLayer& assign);

		void						close(Socket& fill);
		void						reset();

		void						setConn				(const Ws::AppLayer::Conn& appConn);
		void						setCloseCallback	(const Ws::AppLayer::CloseCallback callback);

		Ws::AppLayer::Conn			accessConn();
		Ws::AppLayer::CloseCallback	accessCloseCallback();

	private:
		Ws::AppLayer::Conn			m_appConn;
		Ws::AppLayer::CloseCallback	m_appForceClose;
};


#endif