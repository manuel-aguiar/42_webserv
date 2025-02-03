

#ifndef GLOBALS_HPP

# define GLOBALS_HPP

// C++ headers
# include <cstddef>
# include <string>

//forward declarations
class Clock;
class LogFile;

class Globals
{
	public:
		Globals(Clock* clock = NULL, LogFile* statusFile = NULL, LogFile* errorFile = NULL, LogFile* debugFile = NULL);
		~Globals();

		void				setClockAndLogs(Clock& clock, LogFile& statusFile, LogFile& errorFile, LogFile& debugFile);

		void				logStatus(const std::string& message);
		void				logDebug(const std::string& message);
		void				logError(const std::string& message);

		void				logStatus(const char* message);
		void				logDebug(const char* message);
		void				logError(const char* message);

		Clock*				getClock() const;
		LogFile*			getStatusFile() const;
		LogFile*			getErrorFile() const;
		LogFile*			getDebugFile() const;

		void				setClock(Clock* clock);
		void				setStatusFile(LogFile* statusFile);
		void				setErrorFile(LogFile* errorFile);
		void				setDebugFile(LogFile* debugFile);

	private:

		Clock*				m_clock;
		LogFile*			m_statusFile;
		LogFile*			m_errorFile;
		LogFile*			m_debugFile;

		void				mf_setGlobals();

		Globals(const Globals& copy);
		Globals& operator=(const Globals& assign);
};


#endif
