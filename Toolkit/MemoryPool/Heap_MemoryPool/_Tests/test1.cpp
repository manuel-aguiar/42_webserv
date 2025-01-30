

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>
# include <vector>

// Project headers
# include "../../Heap_MemoryPool/Heap_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../../TestHelpers/TestHelpers.h"
# include "TimerTrackerOld.hpp"

int TestPart1(int testNumber)
{
    // instantiation
    try
    {
        TEST_INTRO(testNumber++);
        Heap_MemoryPool pool(4096);
        
        //Nginx_MemoryPool pool2;   // fails as expected, private default constructor
        //Nginx_MemoryPool copy(pool); // fails as expected, private copy constructor

        Heap_MemoryPool pool2(4096);
        // pool2 = pool1; // fails as expected, private assignment operator
         
        pool.reset();       //all good

        void* ptr = pool.allocate(100);
        (void)ptr;
        
        // RAII, no leaks, everything is destructed

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    try
    {
        TEST_INTRO(testNumber++);
        Heap_MemoryPool pool(4096);
        
        Nginx_PoolAllocator<int, Heap_MemoryPool > alloc(pool);
        std::vector<int, Nginx_PoolAllocator<int, Heap_MemoryPool > > vec(alloc);
        
        vec.reserve(100);

        EXPECT_EQUAL(pool.getFreeSpace(), 4096 - 100 * sizeof(int), "free space is not correct");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    try
    {
        TimerTrackerOld<int, int, std::allocator<int> > tracker(123);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    return (testNumber);
}