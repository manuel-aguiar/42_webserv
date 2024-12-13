

# include "../HttpResponse.hpp"
# include "TestDependencies.hpp"
# include <iostream>

int main(void)
{

	try
	{
		std::cout << "	PASS\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "	FAILED: " << e.what() << '\n';
	}

	return (0);
}

