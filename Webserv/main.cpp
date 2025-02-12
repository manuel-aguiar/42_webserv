


// Project headers
# include "Globals/Globals.hpp"
# include "Globals/Clock/Clock.hpp"
# include "Globals/LogFile/LogFile.hpp"
# include "SignalHandler/SignalHandler.hpp"
# include "ServerConfig/ServerConfig/ServerConfig.hpp"
# include "ServerConfig/BlockFinder/BlockFinder.hpp"
# include "ServerContext/ServerContext.hpp"
# include "CgiModule/Module/Module.hpp"
# include "HttpModule/Module/Module.hpp"
# include "Events/Manager/Manager.hpp"
# include "Connections/Manager/Manager.hpp"

// C++ headers
# include <iostream>
# include <cstdlib>

int maxFdsEstimate(const ServerConfig& config)
{
	int res = 0;
	res += std::atoi(config.getMaxConnections().c_str());
	res += (std::atoi(config.getMaxConcurrentCgi().c_str()) * 3); // read + write + emergency
	res += config.getAllBindAddresses().size();

	res *= std::atoi(config.get)
}

int main(int ac, char** av)
{
    if (ac != 2)
	{
		std::cerr << av[0] << ": Usage: " << av[0] << " <config_file>" << std::endl;
		return (EXIT_FAILURE);
	}

	try
	{

		Clock			clock;
		LogFile			statusFile("status.log");
		LogFile			errorFile("error.log");
		LogFile			debugFile("debug.log");
		Globals			globals(&clock, &statusFile, &errorFile, &debugFile);
		ServerConfig	config(av[1], &globals);
		if (!config.parseConfigFile())
			return (EXIT_FAILURE);
		ServerContext	context;
		BlockFinder		blockFinder;
		Events::Manager	eventManager(200, globals);
		Conn::Manager	connManager(100, config.getAllBindAddresses(), eventManager, globals, context);

		webserver.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << av[0] << ": CRITICAL FAILURE: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	
    return (EXIT_SUCCESS);
}
