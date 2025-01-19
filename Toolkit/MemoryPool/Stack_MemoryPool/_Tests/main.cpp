

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>

// Project
# include "../../Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"


extern int TestPart1(int testNumber);

int main(void)
{
    int testNumber = 1;
    

	std::cout << "\n*************** Stack_MemoryPool tests ***************" << std::endl;
	
    testNumber = TestPart1(testNumber);

    std::cout << "******************************************************\n" << std::endl;

    return 0;
}