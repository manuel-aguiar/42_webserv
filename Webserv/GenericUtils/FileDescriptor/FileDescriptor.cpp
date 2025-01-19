

# include "FileDescriptor.hpp"
# include "../../Globals/Globals.hpp"


int  FileDescriptor::setNonBlocking(t_fd fd)
{
	return (mf_addFlags(fd, O_NONBLOCK));
}

int  FileDescriptor::setCloseOnExec(t_fd fd)
{
	return (mf_addFlags(fd, O_CLOEXEC));
}

int  FileDescriptor::setCloseOnExec_NonBlocking(t_fd fd)
{
	return (mf_addFlags(fd, O_NONBLOCK | O_CLOEXEC));
}


int    FileDescriptor::mf_addFlags(t_fd fd, const int flags)
{
	int currentFlags = ::fcntl(fd, F_GETFL, 0);
	if (currentFlags == -1)
		return (0);
	if (::fcntl(fd, F_SETFL, currentFlags | flags) == -1)
		return (0);
	return (1);
}
