
// Project headers
# include "../EventManager.hpp"
# include "../../../Globals/Globals.hpp"
# include "../../../Event/Event.hpp"
# include "../../../GenericUtils/FileDescriptor/FileDescriptor.hpp"

// C++ headers
# include <iostream>

extern int TestPart1(int testNumber);

int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** EventManager tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);

	std::cout << "**************************************************\n" << std::endl;

	return (0);
}

