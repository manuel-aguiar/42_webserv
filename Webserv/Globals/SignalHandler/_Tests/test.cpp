

# include "../SignalHandler.hpp"
# include "TestDependencies.hpp"
# include <iostream>

int main(void)
{
	/****************************************** */
	std::cout << "Test 1: ";
	try
	{
		SignalHandler handler;
		std::cout << "FAILED, cannot have more than the global instance\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "PASS: " << e.what() << '\n';
	}
	
	return (0);
}

