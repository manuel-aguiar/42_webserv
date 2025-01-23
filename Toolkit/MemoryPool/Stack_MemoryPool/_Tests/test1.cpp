
// test subject
# include "../Stack_MemoryPool.hpp"

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>
# include <vector>
# include <list>

// Project headers
# include "../../Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../Heap_ObjectPool/Heap_ObjectPool.hpp"
# include "../../../_Tests/TestHelpers.h"

int TestPart1(int testNumber)
{
    // instantiation
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";
        Stack_MemoryPool<4096> pool;
        
        //Nginx_MemoryPool pool2;   // fails as expected, private default constructor
        //Nginx_MemoryPool copy(pool); // fails as expected, private copy constructor

        Stack_MemoryPool<4096> pool2;
        // pool2 = pool1; // fails as expected, private assignment operator
         
        pool.reset();       //all good

        void* ptr = pool.allocate(100);
        (void)ptr;
        
        // RAII, no leaks, everything is destructed

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
    
    
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";
        Stack_MemoryPool<4096> pool;
        
        Nginx_PoolAllocator<int, Stack_MemoryPool<4096> > alloc(pool);
        std::vector<int, Nginx_PoolAllocator<int, Stack_MemoryPool<4096> > > vec(alloc);
        
        vec.reserve(100);

        TestHelpers::assertEqual(pool.getFreeSpace(), 4096 - 100 * sizeof(int), "free space is not correct", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


    // I FINALLY FOOLED A STD::LIST TO ALLOCATE MEMORY ON THE STACK
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";
        Stack_MemoryPool<4096> pool;
        
        Nginx_PoolAllocator<std::string, Stack_MemoryPool<4096> > alloc(pool);
        std::list<std::string, Nginx_PoolAllocator<std::string, Stack_MemoryPool<4096> > > list(alloc);
        

        if (pool.getFreeSpace() != 4096)
            throw std::runtime_error("free space is not correct, got: " 
            + TestHelpers::to_string(pool.getFreeSpace()) + " expected: " 
            + TestHelpers::to_string(4096 - 100 * sizeof(int)) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        list.push_back("big string that mallocs just to be sure that RAII is working");
        list.push_back("fits buf");

        if (pool.getFreeSpace() != 4096 - 2 * (sizeof(std::string) + 8 + 8))
            throw std::runtime_error("free space is not correct, got: " 
            + TestHelpers::to_string(pool.getFreeSpace()) + " expected: " 
            + TestHelpers::to_string(4096 - 2 * (sizeof(int) + 4 + 8 + 8)) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


    // I FINALLY FOOLED A STD::LIST TO ALLOCATE MEMORY ON THE STACK
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        const int elementCount = 100;
        const int poolSize = (sizeof(std::string) + 8 + 8) * elementCount;

        Stack_MemoryPool<poolSize> pool;
        
        // a stack memory pool, and using the nginx_allocator to request data from that pool
        Heap_ObjectPool<std::string, Nginx_PoolAllocator<std::string, Stack_MemoryPool<poolSize> > > alloc(100, pool);

        std::list<std::string, Heap_ObjectPool<std::string, Nginx_PoolAllocator<std::string, Stack_MemoryPool<poolSize> > > > list(alloc);
        

        if (pool.getFreeSpace() != poolSize)
            throw std::runtime_error("free space is not correct, got: " 
            + TestHelpers::to_string(pool.getFreeSpace()) + " expected: " 
            + TestHelpers::to_string(4096 - 100 * sizeof(int)) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        for (int i = 0; i < elementCount; i++)
            list.push_back("big string that mallocs just to be sure that RAII is working");

        if (pool.getFreeSpace() != 0)
            throw std::runtime_error("free space is not correct, got: " 
            + TestHelpers::to_string(pool.getFreeSpace()) + " expected: " 
            + TestHelpers::to_string(4096 - 2 * (sizeof(int) + 4 + 8 + 8)) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}