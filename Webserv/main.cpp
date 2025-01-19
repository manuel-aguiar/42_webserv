


// Project headers
# include "Globals/Globals.hpp"
# include "ServerConfig/ServerConfig/ServerConfig.hpp"
# include "ServerManager/ServerManager/ServerManager.hpp"

// C++ headers
# include <iostream>
# include <cstdlib>

/*
	WIP

	Main function for the project
	Estabablish pseudo-global variables
	Load and parse the configuration file
	Instantiate the ServerManager

	RUN
*/
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
		ServerManager	webserver(config, globals);

		webserver.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << av[0] << ": CRITICAL FAILURE: " << e.what() << '\n';
		return (EXIT_FAILURE);
	}
	
    return (EXIT_SUCCESS);
}
