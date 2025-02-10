

#ifndef FILE_HPP

# define FILE_HPP

#include "../Webserver_Definitions.h"
#include <unistd.h>
#include <fcntl.h>

class File
{
	public:
		File(const char* path);
		~File();

		bool		read(void* buffer, size_t size);
		bool		write(const void* buffer, size_t size);
		size_t		size() const;
		const char* path() const;

	private:
		Ws::fd		m_fd;
		size_t 		m_size;
		size_t		offset;

		File(const File& copy);
		File& operator=(const File& copy);

};

#endif
