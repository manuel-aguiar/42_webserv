

#include "../../Webserv/Globals/Globals.hpp"
#include "../../Webserv/Globals/Clock/Clock.hpp"
#include "../../Webserv/Globals/LogFile/LogFile.hpp"
#include "../../Toolkit/Assert/AssertEqual/AssertEqual.h"

#include <pthread.h>

pthread_mutex_t writeMutex;

Globals::Globals(Clock* clock, LogFile* statusFile, LogFile* errorFile, LogFile* debugFile) :
	m_clock(clock),
	m_statusFile(statusFile),
	m_errorFile(errorFile),
	m_debugFile(debugFile)
{
	if (m_clock && m_statusFile && m_errorFile && m_debugFile)
		mf_setGlobals();
	
	pthread_mutex_init(&writeMutex, NULL);
}

Globals::~Globals()
{
	pthread_mutex_destroy(&writeMutex);
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
	pthread_mutex_lock(&writeMutex);
	logStatus(message.c_str());
	pthread_mutex_unlock(&writeMutex);
}

void	Globals::logDebug(const std::string& message)
{
	pthread_mutex_lock(&writeMutex);
	logDebug(message.c_str());
	pthread_mutex_unlock(&writeMutex);
}

void	Globals::logError(const std::string& message)
{
	pthread_mutex_lock(&writeMutex);
	logError(message.c_str());
	pthread_mutex_unlock(&writeMutex);
}

void	Globals::logStatus(const char* message)
{
	pthread_mutex_lock(&writeMutex);
	ASSERT_EQUAL(message != NULL, true, "Globals: logStatus: message cannot be NULL");
	m_statusFile->record(message);
	pthread_mutex_unlock(&writeMutex);
}

void	Globals::logDebug(const char* message)
{
	pthread_mutex_lock(&writeMutex);
	ASSERT_EQUAL(message != NULL, true, "Globals: logDebug: message cannot be NULL");
	m_debugFile->record(message);
	pthread_mutex_unlock(&writeMutex);
}

void	Globals::logError(const char* message)
{
	pthread_mutex_lock(&writeMutex);
	ASSERT_EQUAL(message != NULL, true, "Globals: logError: message cannot be NULL");
	m_errorFile->record(message);
	pthread_mutex_unlock(&writeMutex);
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



