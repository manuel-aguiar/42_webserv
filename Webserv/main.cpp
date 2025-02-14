

// Project headers
# include "ServerConfig/ServerConfig/ServerConfig.hpp"
# include "ServerConfig/DefaultConfig/DefaultConfig.hpp"
# include "Run/WebservRun.h"

// C++ headers
# include <cstdlib> //exit macros

int main(int ac, char** av)
{
    if (ac != 2)
	{
		std::cerr << av[0] << ": Usage: " << av[0] << " <config_file>" << std::endl;
		return (EXIT_FAILURE);
	}
	
	DefaultConfig	defaultConfig;
	ServerConfig	config(av[1], defaultConfig);

	if (!config.parseConfigFile())
		return (EXIT_FAILURE);

	return (WebservRun(av[0], config));
}
