

#include "File.hpp"

File::File(const char *path)
{
	(void)path;
}

File::~File()
{

}

File::File(const File &copy)
{
	(void)copy;
}

File &File::operator=(const File &copy)
{
	(void)copy;
	return (*this);
}


