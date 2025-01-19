

#include "../LogFile/LogFile.hpp"
#include "../Globals.hpp"

LogFile::LogFile(const char* filename, Globals* globals)
{
	(void)filename;
    (void)globals;
}

LogFile::~LogFile()
{
}

void	LogFile::setGlobals(Globals& globals)
{
	(void)globals;
}

void	LogFile::record(const std::string& entry)
{
    (void)entry;
}

void	LogFile::record(const char* entry)
{
	(void)entry;
}
