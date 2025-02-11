

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
			Module(const size_t maxConnections);
			~Module();

			static void    	InitConnection(Conn::Connection& connection);
			static void		CallbackForcedClose(Conn::Connection& connection);

			// AppLayer knows about connection and can call Connection::reset() to bring it back to a clean state
	};
};

#endif