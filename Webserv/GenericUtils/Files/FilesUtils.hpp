
#ifndef FILESUTILS_HPP

# define FILESUTILS_HPP

# include <sys/stat.h>

namespace FilesUtils
{
	typedef enum
	{
		REGULAR_FILE,
		DIRECTORY,
		UNDEFINED,
		NOT_EXIST
	}	FileType;

	bool fileExists(const char* path);
	bool isDirectory(const char* path);
	bool isFile(const char* path);

	FileType getFileType(const char* path);
}

#endif
