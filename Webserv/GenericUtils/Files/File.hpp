

#ifndef FILE_HPP

# define FILE_HPP

#include "../../Ws_Namespace.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>

class File
{
	public:
		File(const char *path);
		~File();

		bool		read(void* buffer, size_t size);
		bool		write(const void* buffer, size_t size);
		size_t		size() const;
		std::string path() const;

	private:
		Ws::fd		m_fd;
		size_t 		m_size;
		size_t		m_offset;

		File(const File& copy);
		File& operator=(const File& copy);

};

#endif
