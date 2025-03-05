#ifndef FILESUTILS_HPP

# define FILESUTILS_HPP

# include <sys/stat.h>

namespace FilesUtils
{
	typedef enum
	{
		REGULAR_FILE,
		DIRECTORY,
		NOT_EXIST,
		UNDEFINED
	}	FileType;

	bool		fileExists(const char* path);
	bool		isDirectory(const char* path);
	bool		isFile(const char* path);
	int			testPath(const char* path);

	FileType	getFileType(const char* path);

}

#endif
