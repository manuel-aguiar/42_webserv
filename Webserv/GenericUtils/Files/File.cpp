

#include "File.hpp"
#include "../BufferView/BufferView.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"
// Needs Testing

// C++ headers
#include <cstring> // std::memset

File::File():
	m_fd(Ws::FD_NONE),
	m_size(0),
	m_path(),
	m_name()
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
	m_name = BufferView();
}

void	File::reset()
{
	this->close();
	m_size = 0;
	m_path.clear();
	m_name = BufferView();
}

bool	File::open(const char *path, int flags, int permissions)
{
	ASSERT_EQUAL(m_fd == Ws::FD_NONE, true, "File: File already open");
	
	m_fd = ::open(path, flags, permissions);
	if (m_fd == Ws::FD_NONE)
		return (false);
	
	m_path = path;

	struct stat info;

	if (::stat(path, &info) != 0)
	{
		this->close();
		return (false);
	}

	m_size = info.st_size;

	size_t pos = m_path.find_last_of('/');
	m_name = BufferView(m_path.c_str(), m_path.size());
	
	if (pos != BufferView::npos)
		m_name = m_name.substr(pos + 1, m_path.size() - pos - 1);

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

	return (::write(m_fd, buffer, size));
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

BufferView
File::name() const
{
	return (m_name);
}

time_t File::getLastModified() const
{
	struct stat info;
	
	if (::stat(m_path.c_str(), &info) != 0)
	{
		std::cerr << "File: error getting last modified time" << std::endl;
		return 0;
	}
	
	return info.st_mtime;
}

off_t	File::seek(off_t offset, int whence)
{
	ASSERT_EQUAL(m_fd != Ws::FD_NONE, true, "File: no file is open for seeking");
	return (::lseek(m_fd, offset, whence));
}

// private
File::File(const File& copy) { static_cast<void>(copy);}
File& File::operator=(const File& copy) { static_cast<void>(copy); return (*this);}
