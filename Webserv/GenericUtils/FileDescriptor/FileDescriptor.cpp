

# include "FileDescriptor.hpp"
# include "../../Globals/Globals.hpp"

static int addFlags(Ws::fd fd, const int flags);

int	FileDescriptor::setBlocking(Ws::fd fd)
{
	return (addFlags(fd, 0));
}

int  FileDescriptor::setNonBlocking(Ws::fd fd)
{
	return (addFlags(fd, O_NONBLOCK));
}

int  FileDescriptor::setCloseOnExec(Ws::fd fd)
{
	return (addFlags(fd, O_CLOEXEC));
}

int  FileDescriptor::setCloseOnExec_NonBlocking(Ws::fd fd)
{
	return (addFlags(fd, O_NONBLOCK | O_CLOEXEC));
}

int    addFlags(Ws::fd fd, const int flags)
{
	int currentFlags = ::fcntl(fd, F_GETFL, 0);
	if (currentFlags == -1)
		return (0);
	if (::fcntl(fd, F_SETFL, currentFlags | flags) == -1)
		return (0);
	return (1);
}
