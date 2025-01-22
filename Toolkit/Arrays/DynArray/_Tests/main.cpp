

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// Project headers
# include "../DynArray.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"

# include "../../../_Tests/TestHelpers.h"

extern int TestPart1(int testNumber);
extern int StressTest(int testNumber);
extern int TestPart3(int testNumber);

int main()
{
    int testNumber = 1;
	/******************* TEST 1 ************************/
	std::cout << "\n*************** DynArray tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);
    testNumber = StressTest(testNumber);
    testNumber = TestPart3(testNumber);

	std::cout << "*********************************************\n" << std::endl;

    return (0);
}

/*
    c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && ./main
*/
