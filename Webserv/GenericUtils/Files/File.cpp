#include "File.hpp"

File::File(const char *path):
	m_size(0),
	m_offset(0)
{
	m_fd = open(path, O_RDWR);
	if (m_fd == -1) {
		std::cerr << "Error opening file: " << path << std::endl;
	}
	else
	{
		struct stat statBuf;
		if (fstat(m_fd, &statBuf) == 0) {
			m_size = statBuf.st_size;
		}
	}
}

File::~File()
{
	if (m_fd != -1) {
		close(m_fd);
	}
}

// File::File(const File &copy)
// {

// }

// File &File::operator=(const File &copy)
// {
// 	(void)copy;
// 	return (*this);
// }

bool	File::read(void* buffer, size_t size)
{
	if (m_fd == -1) {
		return false;
	}

	ssize_t bytesRead = ::read(m_fd, buffer, size);
	if (bytesRead <= 0) {
		return false;
	}

	m_offset += bytesRead;
	return true;
}

bool	File::write(const void* buffer, size_t size)
{
	if (m_fd == -1) {
		return false;
	}

	ssize_t bytesWritten = ::write(m_fd, buffer, size);
	if (bytesWritten < 0) {
		return false;
	}

	m_offset += bytesWritten;
	m_size = std::max(m_size, m_offset);
	return true;
}

// size_t		size() const;
// const char* path() const;

