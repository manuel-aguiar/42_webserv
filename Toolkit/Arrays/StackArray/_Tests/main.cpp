
// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

// Project headers
#include "../StackArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
	int testNumber = 1;
	std::cout << "\n*************** StackArray tests ***************" << std::endl;

	testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);

	std::cout << "*********************************************\n" << std::endl;
}