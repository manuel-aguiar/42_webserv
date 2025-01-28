
#ifndef CONNCONNECTION_HPP

# define CONNCONNECTION_HPP

# include "../../GenericUtils/Webserver_Definitions.h"

// forawrd declarations
class ImplManager;
class Globals;
class ServerContext;
namespace Events {class Manager;}
namespace Events {class Subscription;}

namespace Conn
{
	class Connection
	{
		public:

			void    					close();

			//events
			void						subscribeEvents(bool isCalledFromEventLoop);
			void						unsubscribeEvents(bool isCalledFromEventLoop);
			void						modifyEvents(bool isCalledFromEventLoop);
			Events::Subscription*		accessEvent();


			Ws::Sock::fd				getSocket() const;	
			const Ws::BindInfo&			getBindInfo() const;


			void						setAppLayerConn				(const Ws::AppLayer::Conn& appConn);
			void						setAppLayerCloseCallback	(const Ws::AppLayer::CloseCallback callback);

			Ws::AppLayer::Conn			accessAppLayerConn();
			Ws::AppLayer::CloseCallback	accessAppLayerCloseCallback();

			ServerContext&				accessServerContext();

		protected:
			Connection(ImplManager& connManager);
			~Connection();

			Connection(const Connection& other);
			Connection& operator=(const Connection& other);

			Ws::Sock::fd				m_sockfd;
			Ws::BindInfo				m_info;
			Ws::AppLayer::Conn			m_appConn;
			Ws::AppLayer::CloseCallback	m_appForceClose;
			Events::Subscription*		m_eventSubs;

			ImplManager&				m_connManager;

			void						pf_callAppLayerForceClose();
			void    					pf_reset();

		private:
			ServerContext&				mf_accessServerContext();
			Events::Manager&			mf_accessEventManager();
			Globals&					mf_accessGlobals();

	};
}

#endif