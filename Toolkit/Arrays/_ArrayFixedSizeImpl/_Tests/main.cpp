
// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

// Project headers
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../TestHelpers/TestHelpers.h"

extern int TestPart1(int testNumber);

int main(void)
{
	int testNumber = 1;
	TEST_HEADER("ArrayFixedSize Implementation");

	testNumber = TestPart1(testNumber);

	TEST_FOOTER;

	return (0);
}