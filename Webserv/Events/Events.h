

#ifndef EVENT_H

# define EVENT_H

# include "../GenericUtils/Webserver_Definitions.h"

namespace Events
{

	namespace Monitor
	{
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
		typedef struct epoll_event Event;
	}
}

# define MAX_EPOLL_FDS 1000000

#endif