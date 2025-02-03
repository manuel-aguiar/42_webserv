
#ifndef CONNCONNECTION_HPP

# define CONNCONNECTION_HPP

# include "../../Ws_Namespace.h"

# include "../AppLayer/AppLayer.hpp"
# include "../Monitor/Monitor.hpp"
# include "../Socket/Socket.hpp"

// forawrd declarations
class InternalManager;
class ServerContext;
namespace Events { class Subscription; }

namespace Conn
{
	class Connection
	{
		public:
			void    					close();

			Socket&						accessSocket();
			AppLayer&					accessAppLayer();
			ServerContext&				accessServerContext();
			Events::Manager&			accessEventManager();

			Events::Subscription&		accessEvent();
			void						startMonitoring(bool isCalledFromEventLoop);
			void						stopMonitoring(bool isCalledFromEventLoop);
			void						updateMonitoring(bool isCalledFromEventLoop);

		protected:
			Connection(InternalManager& connManager);
			~Connection();

			Socket						m_socket;
			Monitor						m_monitor;
			AppLayer					m_appLayer;
			InternalManager&			m_connManager;


	};
}

#endif