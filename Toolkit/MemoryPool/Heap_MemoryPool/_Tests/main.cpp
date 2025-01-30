

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>

// Project
# include "../../Heap_MemoryPool/Heap_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"


extern int TestPart1(int testNumber);

int main(void)
{
    int testNumber = 1;
    
    TEST_HEADER("Heap_MemoryPool");
	
    testNumber = TestPart1(testNumber);

    TEST_FOOTER;

    return 0;
}