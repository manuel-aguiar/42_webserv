

// Project headers
# include "../../Webserv/Globals/Globals.hpp"
# include "../../Webserv/Globals/Clock/Clock.hpp"
# include "../../Webserv/Globals/LogFile/LogFile.hpp"
# include "../../Webserv/SignalHandler/SignalHandler.hpp"
# include "../../Webserv/ServerConfig/ServerConfig/ServerConfig.hpp"

// C headers
# include <signal.h>

int SingleServerRun(const char* programName, ServerConfig& config, Globals& globals, const int myServerNumber);
int MultiServerRun(const char* programName, ServerConfig& config, Globals& globals, const int numServers);

int	WebservRun(const char* programName, ServerConfig& config)
{
	Clock			clock;
	LogFile			statusFile("status.log");
	LogFile			errorFile("error.log");
	LogFile			debugFile("debug.log");
	Globals			globals(&clock, &statusFile, &errorFile, &debugFile);

	int numServers = config.getMaxWorkers();

	// setup signal handling
	g_SignalHandler.openSignalListeners(numServers, &globals);
	g_SignalHandler.registerSignal(SIGINT, &globals);
	g_SignalHandler.registerSignal(SIGTERM, &globals);
	g_SignalHandler.registerSignal(SIGQUIT, &globals);
	g_SignalHandler.ignoreSignal(SIGPIPE, &globals);

	if (numServers == 1)
		return (SingleServerRun(programName, config, globals, 0));
	return (MultiServerRun(programName, config, globals, numServers));
}
