

// Project headers
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/TestHelpers.h"

// C++ headers
#include <list>
#include <iostream>
#include <cstring>
#include <stdexcept>

extern int TestPart1(int testNumber);
extern int StressTest(int testNumber);
extern int TestPart3(int testNumber);


int main(void)
{
	int testNumber = 1;

	std::cout << "\n*************** List tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);
	testNumber = StressTest(testNumber);
	testNumber = TestPart3(testNumber);

	std::cout << "******************************************\n" << std::endl;

}