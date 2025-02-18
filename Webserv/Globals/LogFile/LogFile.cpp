

#include "LogFile.hpp"
#include "../Clock/Clock.hpp"
#include "../Globals.hpp"

#include "../../../Toolkit/Assert/AssertEqual/AssertEqual.h"

// C headers
# include <unistd.h>
# include <fcntl.h>

// C++ Headers
# include <cstring>
# include <cerrno>
# include <stdexcept>

LogFile::LogFile(const char* filename, Globals* globals) : m_globals(globals)
{
	ASSERT_EQUAL(filename != NULL, true, "LogFile: filename cannot be NULL");

	m_fd = ::open(filename, O_CREAT | O_APPEND | O_CLOEXEC | O_NONBLOCK | O_RDWR, 0777);
	if (m_fd == -1)
		throw std::runtime_error (std::string("Failed to start LogFile at: open(): ") + std::strerror(errno));
}

LogFile::~LogFile()
{
	::close(m_fd);
}

void	LogFile::setGlobals(Globals& globals)
{
	m_globals = &globals;
}

void	LogFile::record(const std::string& entry)
{
	record(entry.c_str());
}

void	LogFile::record(const char* entry)
{
	int bytesWritten = 0;
	
	ASSERT_EQUAL(m_globals != NULL, true, "LogFile: Globals cannot be NULL");
	m_globals->getClock()->update();

	const char* clockBuf = m_globals->getClock()->get_FormatedTime();

	bytesWritten += write(m_fd, clockBuf, std::strlen(clockBuf));
	bytesWritten += write(m_fd, ": ", 2);
	bytesWritten += write(m_fd, entry, std::strlen(entry));
	bytesWritten += write(m_fd, "\n", 1);

	(void)bytesWritten; // -O3 to avoid warnings
}

LogFile::LogFile() {}
LogFile::LogFile(const LogFile& copy) {(void)copy;}
LogFile& LogFile::operator=(const LogFile& assign) {(void)assign; return (*this);}