

#ifndef HTTPMODULE_HPP

# define HTTPMODULE_HPP

# include "../Ws_Namespace.h"

namespace Conn {class BaseConnection;}
namespace Http {class Connection;}

namespace Http
{
	class Module
	{
		public:
			Module();
			~Module();



			static void    	InitConnection(Conn::BaseConnection& connection);

			// Connection doesn't know http, we need a way for the connection to call the AppLayer to force close in case its needed
			static void		CallbackForcedClose(Conn::BaseConnection& connection);

			// AppLayer knows about connection and can call Connection::reset() to bring it back to a clean state
	};
};

#endif