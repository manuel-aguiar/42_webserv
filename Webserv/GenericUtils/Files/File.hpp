

#ifndef FILE_HPP

# define FILE_HPP

#include "../../Ws_Namespace.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <string>

// Needs Testing

class File
{
	public:
		File(const char *path);
		File(const File& copy);
		~File();

		void				open(const char *path);
		void				close();

		bool				read(void* buffer, size_t size);
		bool				write(const void* buffer, size_t size);
		size_t				size() const;
		const std::string	&path() const;
		Ws::fd				fd() const;
		size_t				offset() const;

	private:
		Ws::fd		m_fd;
		size_t		m_size;
		size_t		m_offset;
		std::string	m_path;


		File& operator=(const File& copy);

};

#endif
