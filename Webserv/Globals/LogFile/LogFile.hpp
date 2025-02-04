

#ifndef LOGFILE_HPP

# define LOGFILE_HPP

// C++ headers
# include <cstddef> // defines NULL
# include <string>

class Globals;

// not making the full namespace a dependency when i just want the fd typedef
namespace Ws {typedef int fd;}

class LogFile
{

	public:
		LogFile(const char* filename, Globals* globals = NULL);
		~LogFile();

		void		setGlobals(Globals& globals);

		void		record(const std::string& entry);
		void		record(const char* entry);


	private:
		Ws::fd		m_fd;
		Globals*	m_globals;


		LogFile();
		LogFile(const LogFile& copy);
		LogFile& operator=(const LogFile& assign);

};



#endif
