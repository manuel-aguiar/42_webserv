

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
		File();
		~File();

		bool				open(const char *path, int flags, int permissions);
		void				close();

		void				reset();
		int					read(void* buffer, size_t size);
		int					write(const void* buffer, size_t size);
		size_t				size() const;
		const std::string&	path() const;
		Ws::fd				fd() const;
		time_t				getLastModified() const;
		off_t				seek(off_t offset, int whence);
	private:
		Ws::fd				m_fd;
		size_t				m_size;
		std::string			m_path;


		File(const File& copy);
		File& operator=(const File& copy);

};

#endif
