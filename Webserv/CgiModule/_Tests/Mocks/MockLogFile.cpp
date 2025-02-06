

#include "../../../Globals/Globals.hpp"
#include "../../../Globals/Clock/Clock.hpp"
#include "../../../Globals/LogFile/LogFile.hpp"

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
