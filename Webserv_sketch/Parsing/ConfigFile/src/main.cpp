#include "utils.hpp"
#include "ConfigHandler.hpp"
#include "ServerConfig.hpp"


int main(int argc, char **argv)
{
	ConfigHandler	configHandler;

	// Set config-file path
	if (argc == 2)
		configHandler.setConfigPath(argv[1]);
	else
		configHandler.setConfigPath(DEFAULT_CONFIG_PATH);
	
	// Parse config file
	if (!configHandler.parse_config_file())
		return (1);
	else
	{
		std::cout << "Config parsing done! Showing stored information:" << std::endl;
		configHandler.print_servers_config();
	}
}