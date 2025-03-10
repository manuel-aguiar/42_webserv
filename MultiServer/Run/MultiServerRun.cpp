

// Project headers
# include "../../Webserv/Globals/Globals.hpp"
# include "../../Webserv/SignalHandler/SignalHandler.hpp"
# include "../../Webserv/ServerConfig/ServerConfig/ServerConfig.hpp"

# include "../../Toolkit/ThreadPool/ThreadPool.hpp"

// C++ headers
# include <cstdlib>
# include <cstdio> // EXIT macros

// C headers
# include <signal.h>
# include <unistd.h> // sleep

extern int SingleServerRun(const char* programName, ServerConfig& config, Globals& globals, const int myServerNumber);


class MultiServerTask : public IThreadTask
{
	public:
		MultiServerTask(const char* programName, ServerConfig& config, Globals& globals, const int myServerNumber) : 
			m_programName(programName), 
			m_config(config), 
			m_globals(globals),
			m_myServerNumber(myServerNumber) {}
		~MultiServerTask() {}
		
		void execute()
		{
			SingleServerRun(m_programName, m_config, m_globals, m_myServerNumber);
		}
	private:
		const char*		m_programName;
		ServerConfig&	m_config;
		Globals&		m_globals;
		const int		m_myServerNumber;
};

int	MultiServerRun(const char* programName, ServerConfig& config, Globals& globals, const int numServers)
{
	sigset_t 					threadSigSet;
	DynArray<MultiServerTask> 	tasks;


	/* Disable SIGINT and SIGQUIT for the threadpool workers*/
	sigemptyset(&threadSigSet);
	sigaddset(&threadSigSet, SIGINT);
	sigaddset(&threadSigSet, SIGQUIT);
	sigaddset(&threadSigSet, SIGTERM);

	pthread_sigmask(SIG_BLOCK , &threadSigSet, NULL);		// explicitely block sigint/quit for new threads		UNPROTECTED
	ThreadPoolHeap servers(numServers, numServers);
	pthread_sigmask(SIG_UNBLOCK, &threadSigSet, NULL);

	tasks.reserve(numServers);
	for (int i = 0; i < numServers; i++)
	{
		tasks.emplace_back(programName, config, globals, i);
		servers.addTask(tasks[i], true);
	}

	while (!g_SignalHandler.getSignal())
		::usleep(1000);

	servers.destroy(true);

	return (EXIT_SUCCESS);
}