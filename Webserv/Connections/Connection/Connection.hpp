

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

// Toolkit headers
# include "../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.hpp"

// Project headers
# include "../GenericUtils/Webserver_Definitions.h"

// forward declarations
class ListeningSocket;
class ServerContext;
class ImplManager;
namespace Events { class Manager; }
namespace Events { class Subscription; }

namespace Conn
{
	class Connection
	{
		public:
			Connection(	Events::Manager& eventManager, 
						ImplManager& connManager, 
						ServerContext& serverContext);
			~Connection();
			Connection(const Connection& other);
			Connection& operator=(const Connection& other);

			typedef void 				(*Init)(Connection*);

			void						subscribeEvents();
			void						unsubscribeEvents();
			void						modifyEvents();	

		//methods
			void    					init();
			void    					reset();
			void    					close();

		//getters
			const Ws::Sock::addr&		getAddr()			const;
			const ListeningSocket*		getListener()		const;
			const Ws::Sock::addr&		getAddr() const;

		//setters
			void						setAppLayerConn		(const Ws::AppLayer::Conn& appConn);
			void						setAppLayerForceClose(const Ws::AppLayer::ForceCloseCallback callback);
			void						setSocket			(const t_socket sockfd);
			void						setAddr				(const Ws::Sock::addr& addr);
			void						setAddrlen			(const t_socklen addrlen);
			void						setListener			(ListeningSocket& listener);
			void						setEventSubs		(Events::Subscription* subs);
			
			void						CallUserForceClose();
		//accessors
			Ws::AppLayer::Conn			accessAppLayerConn();
			Events::Subscription*		accessEventSubs();
			Events::Manager&			accessEventManager();
			ServerContext&				accessServerContext();


		private:
			Ws::Sock::fd				m_sockfd;
			Ws::Sock::addr				m_addr;
			Ws::Sock::addrlen			m_addrlen;
			Events::Subscription*		m_eventSubs;
			Ws::AppLayer::Conn			m_appConn;
			Ws::AppLayer::ForceCloseCallback	
										m_appForceClose;
			ImplManager&				m_connManager;
			Events::Manager&			m_eventManager;
			ServerContext&				m_context;
																					// set by the listening socket
	};
}


#endif
