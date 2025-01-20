

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <list>

// Project headers
# include "../StackCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"

# include "../../../TestHelpers/TestHelpers.h"

extern int TestPart1(int testNumber);
extern int TestPart2(int testNumber);
extern int TestPart3(int testNumber);
extern int TestPart4(int testNumber);


int main()
{
    int testNumber = 1;
    
	std::cout << "\n*************** StackCircularQueue tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);
    testNumber = TestPart2(testNumber);
    testNumber = TestPart3(testNumber);
    testNumber = TestPart4(testNumber);

	std::cout << "*********************************************\n" << std::endl;

    return (0);
}
