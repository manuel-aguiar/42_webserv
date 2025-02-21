#include "File.hpp"

File::File(const char *path):
	m_size(0),
	m_offset(0),
	m_path(std::string(path))
{
	m_fd = open(path, O_RDWR);
	if (m_fd == Ws::FD_NONE) 
		std::cerr << "Error opening file: " << path << std::endl;
	else
	{
		struct stat statBuf;
		if (fstat(m_fd, &statBuf) == 0)
			m_size = statBuf.st_size;
	}
}

File::~File()
{
	if (m_fd != Ws::FD_NONE) {
		close(m_fd);
	}
}

bool	File::read(void* buffer, size_t size)
{
	if (m_fd == Ws::FD_NONE) 
		return (false);

	ssize_t bytesRead = ::read(m_fd, buffer, size);
	if (bytesRead <= 0) 
		return (false);

	m_offset += bytesRead;
	return (true);
}

bool	File::write(const void* buffer, size_t size)
{
	if (m_fd == Ws::FD_NONE)
		return (false);

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