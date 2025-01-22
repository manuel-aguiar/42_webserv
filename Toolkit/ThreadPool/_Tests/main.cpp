

//test target
# include "../ThreadPool.hpp"


// Project headers
# include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/ToolkitBase.hpp"
# include "../../_Tests/ToolkitDerived.hpp"
# include "../../_Tests/TestHelpers.h"

// C++ headers
# include <unistd.h>
# include <cstring>
# include <sstream>
# include <cstdlib>
# include <iostream>

extern int TestPart1(int testNumber);
extern int StressTest(int testNumber);
extern int TestPart3(int testNumber);
extern int TestPart4(int testNumber);

int main(void)
{

    int testNumber = 1;
	/******************* TEST 1 ************************/
	std::cout << "\n************ ThreadPool tests *************" << std::endl;
	
	

    testNumber = TestPart1(testNumber);
	testNumber = StressTest(testNumber);
	testNumber = TestPart3(testNumber);
	testNumber = TestPart4(testNumber);

	std::cout << "*******************************************\n" << std::endl;
	
	return (0);
}





