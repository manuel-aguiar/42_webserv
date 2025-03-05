#include "FilesUtils.hpp"
#include <unistd.h>

// Tests if path exists, is accessible, and has read/write permissions
// Returns: -1 if path not accessible, 0 if no read/write permissions, 1 if path exists and has permissions
int FilesUtils::testPath(const char* path)
{
	struct stat buffer;

	if (access(path, F_OK) != 0)
		return -1; // Path not accessible
	
	if (stat(path, &buffer) != 0)
		return -1;

	if (access(path, R_OK | W_OK /*| X_OK*/) != 0)
		return 0; // No read/write permissions

	return 1; // Path exists and has permissions
}

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
