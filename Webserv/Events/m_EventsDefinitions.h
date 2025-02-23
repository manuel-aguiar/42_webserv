

#ifndef M_EVENTSDEFINITIONS_H

# define M_EVENTSDEFINITIONS_H

# ifdef __linux__
#  include <sys/epoll.h>
# endif

# ifdef __APPLE__
#  include <sys/event.h>
# endif

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
		typedef int Mask;
		typedef struct epoll_event Event;
	}
	typedef struct epoll_event EpollEvent;
}

#endif
