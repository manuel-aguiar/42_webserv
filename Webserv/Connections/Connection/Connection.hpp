
#ifndef CONNCONNECTION_HPP

# define CONNCONNECTION_HPP

# include "../../Ws_Namespace.h"

# include "../../Events/Subscription/Subscription.hpp"
# include "../AppLayer/AppLayer.hpp"
# include "../Monitor/Monitor.hpp"
# include "../Socket/Socket.hpp"

// forawrd declarations
class InternalManager;
class ServerContext;


namespace Conn
{
	class Connection
	{
		public:
			void    					close();

			// server context access
			ServerContext&				accessServerContext();

			// socket info access
			Ws::Sock::fd				sock_getFd() const;
			const Ws::BindInfo&			sock_getBindInfo() const;

			// appLayer access
			Ws::AppLayer::Conn			appLayer_accessConn();
			Ws::AppLayer::CloseCallback	appLayer_accessCloseCallback();
			void						appLayer_setConn			(const Ws::AppLayer::Conn& appConn);
			void						appLayer_setCloseCallback	(const Ws::AppLayer::CloseCallback callback);

			// event controls
			Events::Monitor::Mask		events_getTriggeredEvents() const;
			void						events_setMonitoredEvents	(const Events::Monitor::Mask flags);
			void						events_setUser				(const Events::Subscription::User data);
			void						events_setCallback			(const Events::Subscription::Callback handler);

			void						events_startMonitoring(bool isCalledFromEventLoop);
			void						events_stopMonitoring(bool isCalledFromEventLoop);
			void						events_updateMonitoring(bool isCalledFromEventLoop);

			// for if we were to implement proxy pass, access eventManager to connect
			Events::Manager&			accessEventManager();

		protected:
			Connection(InternalManager& connManager);
			~Connection();

			Socket						m_socket;
			Monitor						m_monitor;
			AppLayer					m_appLayer;
			InternalManager&			m_connManager;

			Socket&						mf_accessSocket();
			AppLayer&					mf_accessAppLayer();

	};
}

#endif