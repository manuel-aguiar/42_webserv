
// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

// Project headers
#include "../StackArray.hpp"
#include "../../../Toolkit/_Tests/ToolkitDummy.hpp"
#include "../../../Toolkit/_Tests/ToolkitBase.hpp"
#include "../../../Toolkit/_Tests/ToolkitDerived.hpp"

extern int TestPart0(int testNumber);
extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);

int main(void)
{
	int testNumber = 1;
	TEST_HEADER("StackArray");

	testNumber = TestPart0(testNumber);
	testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);

	TEST_FOOTER;

	return (0);
}