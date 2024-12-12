

# include "../EventManager.hpp"
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

	std::cout << "Test1: ";
	try
	{
		EventManager manager(globals);

		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	return (0);
}

