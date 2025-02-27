


#include "../../../../GenericUtils/Files/File.hpp"
#include "../../../../../Toolkit/Assert/AssertEqual/AssertEqual.h"
// Needs Testing

# include <algorithm> // std::min

extern size_t g_maxFileWrite;

File::File():
	m_fd(Ws::FD_NONE),
	m_size(0)
{
}

File::~File()
{
	if (m_fd != Ws::FD_NONE)
		::close(m_fd);
}

void	File::close()
{
	if (m_fd != Ws::FD_NONE)
		::close(m_fd);
	m_fd = Ws::FD_NONE;
	m_size = 0;
	m_path.clear();
}

void	File::reset()
{
	this->close();
	m_size = 0;
	m_path.clear();
}

bool	File::open(const char *path, int flags, int permissions)
{
	ASSERT_EQUAL(m_fd == Ws::FD_NONE, true, "File: File already open");
	m_fd = ::open(path, flags, permissions);
	if (m_fd == Ws::FD_NONE) 
		std::cerr << "Error opening file: " << path << std::endl;
	
	m_path = path;

	struct stat info;

	if (::stat(path, &info) != 0)
	{
		std::cerr << "File: error getting file size" << std::endl;
		this->close();
		return (false);
	}
	
	m_size = info.st_size;

	return (m_fd != Ws::FD_NONE);
}

int	File::read(void* buffer, size_t size)
{
	ASSERT_EQUAL(m_fd != Ws::FD_NONE, true, "File: no file is open for reading");
	return (::read(m_fd, buffer, size));
}

int	File::write(const void* buffer, size_t size)
{
	ASSERT_EQUAL(m_fd != Ws::FD_NONE, true, "File: no file is open for writing");
	return (::write(m_fd, buffer, std::min(size, g_maxFileWrite)));
}

size_t File::size() const
{
    return (m_size);
}

const std::string &File::path() const
{
    return (m_path);
}

Ws::fd File::fd() const
{
	return (m_fd);
}

// private
File::File(const File& copy) { static_cast<void>(copy);}
File& File::operator=(const File& copy) { static_cast<void>(copy); return (*this);}
