

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>

// Project
# include "../../Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../../TestHelpers/TestHelpers.h"

extern int TestPart1(int testNumber);

int main(void)
{
    int testNumber = 1;
    
    TEST_HEADER("Stack_MemoryPool");
	
    testNumber = TestPart1(testNumber);

    TEST_FOOTER;

    return 0;
}