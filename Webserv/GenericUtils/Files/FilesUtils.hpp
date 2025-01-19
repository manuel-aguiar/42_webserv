

#ifndef FILESUTILS_HPP

# define FILESUTILS_HPP

# include <sys/stat.h>

class FilesUtils
{
	public:
		static bool fileExists(const char* path);
		static bool isDirectory(const char* path);
		static bool isFile(const char* path);
};

#endif
