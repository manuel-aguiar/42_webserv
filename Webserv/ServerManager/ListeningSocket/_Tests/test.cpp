

# include "../ListeningSocket.hpp"
# include "TestDependencies.hpp"
# include <iostream>

# include "../../../Globals/Globals.hpp"
# include "../../../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../../ServerManager.hpp"
int main(void)
{

	/******************************************/
	//setup
	Globals				globals(NULL, NULL, NULL, NULL);
	Nginx_MemoryPool*	pool = Nginx_MemoryPool::create(4096, 1);
	ServerConfig		config(NULL, &globals);
	ServerManager		manager(config, globals);
	{
		ServerWorker		worker(manager, 0, *pool, globals);

		std::cout << "Test1: ";
		try
		{
			t_addrinfo		addrInfo = (t_addrinfo){};
			ListeningSocket listener(worker, *pool, addrInfo, 10, globals);

			std::cout << "	PASS\n";
		}
		catch(const std::exception& e)
		{
			std::cerr << "	FAILED: " << e.what() << '\n';
		}
	}
	
	
	pool->destroy();

	return (0);
}

