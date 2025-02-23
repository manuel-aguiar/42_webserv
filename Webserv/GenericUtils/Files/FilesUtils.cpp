

#include "FilesUtils.hpp"

bool FilesUtils::fileExists(const char* path)
{
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

bool FilesUtils::isDirectory(const char* path)
{
	struct stat buffer;
	if (stat(path, &buffer) == 0)
		return (S_ISDIR(buffer.st_mode));
	return (false);
}

bool FilesUtils::isFile(const char* path)
{
	struct stat buffer;
	if (stat(path, &buffer) == 0)
		return (S_ISREG(buffer.st_mode));
	return (false);
}

FilesUtils::FileType FilesUtils::getFileType(const char* path)
{
	struct stat buffer;
	
	if (stat(path, &buffer) != 0)
		return (NOT_EXIST);
	if (S_ISDIR(buffer.st_mode))
		return (DIRECTORY);
	if (S_ISREG(buffer.st_mode))
		return (REGULAR_FILE);
	return (UNDEFINED);
}
