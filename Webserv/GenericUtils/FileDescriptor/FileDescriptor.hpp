

#ifndef FILEDESCRIPTOR_HPP

# define FILEDESCRIPTOR_HPP

# include <fcntl.h>
# include "../Webserver_Definitions.h"

class Globals;

class FileDescriptor
{
	public:
		static int  setNonBlocking					(t_fd fd);
		static int  setCloseOnExec					(t_fd fd);
		static int  setFdFlags						(t_fd fd, const int flags);
		static int  setCloseOnExec_NonBlocking		(t_fd fd);
	private:
		static int mf_addFlags						(t_fd fd, const int flags);
};

#endif