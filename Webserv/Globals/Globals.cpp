

#include "Globals.hpp"
#include "Clock/Clock.hpp"
#include "LogFile/LogFile.hpp"

Globals::Globals(Clock* clock, LogFile* statusFile, LogFile* errorFile, LogFile* debugFile) :
	m_clock(clock),
	m_statusFile(statusFile),
	m_errorFile(errorFile),
	m_debugFile(debugFile)
{
	if (m_clock && m_statusFile && m_errorFile && m_debugFile)
		mf_setGlobals();
}

void	Globals::setClockAndLogs(Clock& clock, LogFile& statusFile, LogFile& errorFile, LogFile& debugFile)
{
	m_clock = &clock;
	m_statusFile = &statusFile;
	m_errorFile = &errorFile;
	m_debugFile = &debugFile;

	mf_setGlobals();
}

void	Globals::logStatus(const std::string& message)
{
	assert(m_statusFile != NULL);
	m_statusFile->record(message);
}

void	Globals::logDebug(const std::string& message)
{
	assert(m_debugFile != NULL);
	m_debugFile->record(message);
}

void	Globals::logError(const std::string& message)
{
	assert(m_errorFile != NULL);
	m_errorFile->record(message);
}

void	Globals::logStatus(const char* message)
{
	assert(m_statusFile != NULL);
	//CUSTOM_ASSERT(message != NULL, "SKILL ISSUE");
	m_statusFile->record(message);
}

void	Globals::logDebug(const char* message)
{
	assert(m_debugFile != NULL);
	m_debugFile->record(message);
}

void	Globals::logError(const char* message)
{
	assert(m_errorFile != NULL);
	m_errorFile->record(message);
}

inline
void    Globals::mf_setGlobals()
{
	m_clock->setGlobals(*this);
	m_statusFile->setGlobals(*this);
	m_errorFile->setGlobals(*this);
	m_debugFile->setGlobals(*this);
}

Clock*              Globals::getClock() const { return (m_clock); }
LogFile*            Globals::getStatusFile() const { return (m_statusFile); }
LogFile*            Globals::getErrorFile() const { return (m_errorFile); }
LogFile*            Globals::getDebugFile() const { return (m_debugFile); }
void 			  	Globals::setClock(Clock* clock) { m_clock = clock; m_clock->setGlobals(*this); }
void 			  	Globals::setStatusFile(LogFile* statusFile) { m_statusFile = statusFile; m_statusFile->setGlobals(*this);}
void 			  	Globals::setErrorFile(LogFile* errorFile) { m_errorFile = errorFile; m_errorFile->setGlobals(*this);}
void 			  	Globals::setDebugFile(LogFile* debugFile) { m_debugFile = debugFile; m_debugFile->setGlobals(*this);}

Globals::~Globals()
{

}


