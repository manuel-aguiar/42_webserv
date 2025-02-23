


#include "File.hpp"
#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"
// Needs Testing

File::File():
	m_fd(Ws::FD_NONE),
	m_size(0),
	m_offset(0)
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
}

void	File::reset()
{
	this->close();
	m_size = 0;
	m_offset = 0;
}

void	File::open(const char *path)
{
	ASSERT_EQUAL(m_fd == Ws::FD_NONE, true, "File: File already open");
	m_fd = ::open(path, O_RDWR);
	if (m_fd == Ws::FD_NONE) 
		std::cerr << "Error opening file: " << path << std::endl;
}


bool	File::read(void* buffer, size_t size)
{
	ASSERT_EQUAL(m_fd != Ws::FD_NONE, true, "File: no file is open for reading");

	ssize_t bytesRead = ::read(m_fd, buffer, size);
	if (bytesRead <= 0) 
		return (false);

	m_offset += bytesRead;
	return (true);
}

bool	File::write(const void* buffer, size_t size)
{
	ASSERT_EQUAL(m_fd != Ws::FD_NONE, true, "File: no file is open for writing");

	ssize_t bytesWritten = ::write(m_fd, buffer, size);
	if (bytesWritten < 0)
		return (false);
	m_offset += bytesWritten;
	m_size = std::max(m_size, m_offset);
	return (true);
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

size_t File::offset() const
{
	return (m_offset);
}

// private
File::File(const File& copy) { static_cast<void>(copy);}
File& File::operator=(const File& copy) { static_cast<void>(copy); return (*this);}
