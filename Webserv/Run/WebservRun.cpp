

// Project headers
# include "../Globals/Globals.hpp"
# include "../Globals/Clock/Clock.hpp"
# include "../Globals/LogFile/LogFile.hpp"
# include "../SignalHandler/SignalHandler.hpp"
# include "../ServerConfig/ServerConfig/ServerConfig.hpp"

// C headers
# include <signal.h>


extern int SingleServerRun(const char* programName, ServerConfig& config, Globals& globals, const int myServerNumber);

int	WebservRun(const char* programName, ServerConfig& config)
{
	Clock			clock;
	LogFile			statusFile("status.log");
	LogFile			errorFile("error.log");
	LogFile			debugFile("debug.log");
	Globals			globals(&clock, &statusFile, &errorFile, &debugFile);

	// setup signal handling
	g_SignalHandler.openSignalListeners(1, &globals);
	g_SignalHandler.registerSignal(SIGINT, &globals);
	g_SignalHandler.registerSignal(SIGTERM, &globals);
	g_SignalHandler.registerSignal(SIGQUIT, &globals);
	g_SignalHandler.ignoreSignal(SIGPIPE, &globals);

	return (SingleServerRun(programName, config, globals, 0));
}
