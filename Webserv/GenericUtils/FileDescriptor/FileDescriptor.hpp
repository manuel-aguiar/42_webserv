

#ifndef FILEDESCRIPTOR_HPP

# define FILEDESCRIPTOR_HPP

# include "../Webserver_Definitions.h"
# include <fcntl.h>

namespace FileDescriptor
{
	int  setNonBlocking                  (t_fd fd);
	int  setCloseOnExec                  (t_fd fd);
	int  setFdFlags                      (t_fd fd, const int flags);
	int  setCloseOnExec_NonBlocking      (t_fd fd);
};

#endif