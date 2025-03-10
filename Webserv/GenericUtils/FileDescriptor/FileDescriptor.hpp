

#ifndef FILEDESCRIPTOR_HPP

# define FILEDESCRIPTOR_HPP

# include "../../Ws_Namespace.h"
# include <fcntl.h>

namespace FileDescriptor
{
	int  setBlocking                   		(Ws::fd fd);
	int  setNonBlocking                  	(Ws::fd fd);
	int  setCloseOnExec                  	(Ws::fd fd);
	int  setFdFlags                      	(Ws::fd fd, const int flags);
	int  setCloseOnExec_NonBlocking      	(Ws::fd fd);
}

#endif
