

#ifndef EVENT_H

# define EVENT_H

# include "../GenericUtils/Webserver_Definitions.h"

namespace Event
{
	class Manager;

	namespace Flags {
		enum
		{
			NONE 		  	= 0,
			READ            = EPOLLIN,
			WRITE           = EPOLLOUT,
			ERROR           = EPOLLERR,
			HANGUP          = EPOLLHUP,
			RDHANGUP        = EPOLLRDHUP,
			EDGE_TRIGGERED  = EPOLLET
		};
		typedef int Type;
	}
}

#endif