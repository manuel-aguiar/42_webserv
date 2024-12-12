

# include "../ConnectionManager.hpp"
# include "TestDependencies.hpp"
# include <iostream>
# include "../../../Globals/Globals.hpp"

int main(void)
{
	/***************************************** */
	Clock			clock;
	LogFile			statusFile("status.log");
	LogFile			errorFile("error.log");
	LogFile			debugFile("debug.log");
	Globals			globals(&clock, &statusFile, &errorFile, &debugFile);

	Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096, 1);

	std::cout << "Test1: ";
	try
	{
		ConnectionManager manager(1, *pool, globals);
		
		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}
	
	pool->destroy();

	return (0);
}

