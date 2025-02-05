

//test target
# include "../ThreadPool.hpp"


// Project headers
# include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/ToolkitBase.hpp"
# include "../../_Tests/ToolkitDerived.hpp"
# include "../../TestHelpers/TestHelpers.h"

// C++ headers
# include <unistd.h>
# include <cstring>
# include <sstream>
# include <cstdlib>
# include <iostream>

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);
extern int TestPart3(int testNumber);
extern int TestPart4(int testNumber);

int main(void)
{

    int testNumber = 1;
	/******************* TEST 1 ************************/

	TEST_HEADER("ThreadPool");

    testNumber = TestPart1(testNumber);
	testNumber = TestPart2(testNumber);
	testNumber = TestPart3(testNumber);
	testNumber = TestPart4(testNumber);

	TEST_FOOTER;
		
	return (0);
}





