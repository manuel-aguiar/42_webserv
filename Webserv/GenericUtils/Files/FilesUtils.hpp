
#ifndef FILESUTILS_HPP

# define FILESUTILS_HPP

# include <sys/stat.h>

namespace FilesUtils
{
	bool fileExists(const char* path);
	bool isDirectory(const char* path);
	bool isFile(const char* path);
};

#endif
