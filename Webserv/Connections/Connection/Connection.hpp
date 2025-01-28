
#ifndef CONNCONNECTION_HPP

# define CONNCONNECTION_HPP

# include "../../GenericUtils/Webserver_Definitions.h"

# include "../AppLayer/AppLayer.hpp"
# include "../Monitor/Monitor.hpp"
# include "../Socket/Socket.hpp"

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

			Socket&						accessSocket();
			AppLayer&					accessAppLayer();
			Monitor&					accessMonitor();

			ServerContext&				accessServerContext();

		protected:
			Connection(ImplManager& connManager);
			~Connection();

			Socket						m_socket;
			Monitor						m_monitor;
			AppLayer					m_appLayer;
			ImplManager&				m_connManager;

	};
}

#endif