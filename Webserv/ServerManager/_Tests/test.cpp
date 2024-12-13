

# include "../ServerManager.hpp"
# include "TestDependencies.hpp"
# include <iostream>

# include "../../ServerConfig/ServerConfig/ServerConfig.hpp"

int main(void)
{
	/**********************************************/

	std::cout << "Test1: ";

	/* Just to check that i can spawn the servermanager, somehow */

	try
	{
		Globals 		globals(NULL, NULL, NULL, NULL);
		ServerConfig 	config(NULL, &globals);
		ServerManager 	manager(config, globals);

		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}
	


	return (0);
}

