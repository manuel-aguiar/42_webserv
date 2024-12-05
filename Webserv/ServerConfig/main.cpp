#include "./ServerConfig/ServerConfig.hpp"
#include "./ServerBlock/ServerBlock.hpp"
#include "./ServerLocation/ServerLocation.hpp"
#include "./DefaultConfig/DefaultConfig.hpp"

#include <iostream>

#include "../GenericUtils/Webserver_Definitions.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	ServerConfig conf("./_Tests/TestFiles/Pass_webserv.conf", NULL);
	
	if (!conf.parseConfigFile())
		return (1);
	else
	{
		std::cout << "Config parsing done! Showing stored information:" << std::endl;
		conf.printConfigs();
	}
}