

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>
# include <list>

// Project headers
# include "../Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../Heap_ObjectPool/Heap_ObjectPool.hpp"
# include "../../../_Tests/TestHelpers.h"
# include "TestAllocator.tpp"

int TestPart1(int testNumber)
{
    // instantiation
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";
        Nginx_MemoryPool pool(4096, 1);
        
        //Nginx_MemoryPool pool2;   // fails as expected, private default constructor
        //Nginx_MemoryPool copy(pool); // fails as expected, private copy constructor

        Nginx_MemoryPool pool2(4096, 1);
        // pool2 = pool1; // fails as expected, private assignment operator
         
        pool.reset();       //all good
        pool.destroy();     // all good

        void* ptr = pool.allocate(100);
        (void)ptr;
        
        // RAII, no leaks, everything is destructed

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    // instantiation
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        size_t counters[TestAllocator<t_byte>::E_TOTAL_COUNTERS] = {0};

        // inspecting the internals of the Nginx_MemoryPool by infusing an allocator that counts
        TestAllocator<t_byte> alloc(counters);
        Impl_Nginx_MemoryPool<t_byte, TestAllocator<t_byte> > pool(4096, 1, alloc);

        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], (size_t)1, "alloc count failed", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], (size_t)4096, "alloc bytes failed", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


    // instantiation
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        size_t counters[TestAllocator<t_byte>::E_TOTAL_COUNTERS] = {0};

        // dark magic, using Nginx_MemoryPool to allocate ints instead of bytes, don't do this at home
        TestAllocator<t_byte> alloc(counters);
        Impl_Nginx_MemoryPool<int, TestAllocator<t_byte> > pool(100, 1, alloc);

        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], (size_t)1, "alloc count failed", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], (size_t)(100 * (sizeof(int))), "alloc bytes failed", __FILE__, __LINE__, __FUNCTION__);

        void* bigBlock = pool.allocate(200);
        (void)bigBlock;

        //big block, will overwelm the pool

        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], (size_t)2, "alloc count failed", __FILE__, __LINE__, __FUNCTION__);

        size_t expectedBytes = (100 + 200 + 16 / sizeof(int)) * (sizeof(int)); //16 / sizeof(int)(how many ints i have to allocate to have place for the big block struct)
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], expectedBytes, "alloc bytes failed", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    try     // using the memory pool to allocate memory for linked list nodes
    {
        std::cout << "TEST " << testNumber++ << ": ";

        size_t counters[TestAllocator<t_byte>::E_TOTAL_COUNTERS] = {0};

        TestAllocator<t_byte> stdAllocator(counters);

        typedef Impl_Nginx_MemoryPool<t_byte, TestAllocator<t_byte> >   monitoredPool;
        typedef Nginx_PoolAllocator<int, monitoredPool >                monitoredAllocator;
        typedef Heap_ObjectPool<int, monitoredAllocator >               monitoredObjPool;
        typedef std::list<int, monitoredObjPool >                       monitoredList;

        monitoredPool       pool(4096, 1, stdAllocator);
        monitoredAllocator  allocator(pool);
        monitoredObjPool    objPool(100, allocator);        //obj pool for 100 list nodes of integers
        monitoredList       list(objPool);

        //same result, didn't overwelm the pool
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], (size_t)1, "alloc count failed", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], (size_t)4096, "alloc bytes failed", __FILE__, __LINE__, __FUNCTION__);


        // inserting integers
        for (size_t i = 0; i < 100; ++i)
            list.push_back(i);


        // should not trigger any call to new, there is plenty of space in the first block of the pool
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], (size_t)1, "alloc count failed", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], (size_t)4096, "alloc bytes failed", __FILE__, __LINE__, __FUNCTION__);


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    try     // using the memory pool to allocate memory for linked list nodes
    {
        std::cout << "TEST " << testNumber++ << ": ";

        size_t counters[TestAllocator<t_byte>::E_TOTAL_COUNTERS] = {0};

        TestAllocator<t_byte> stdAllocator(counters);

        typedef Impl_Nginx_MemoryPool<t_byte, TestAllocator<t_byte> >   monitoredPool;
        typedef Nginx_PoolAllocator<int, monitoredPool >                monitoredAllocator;
        typedef Heap_ObjectPool<int, monitoredAllocator >               monitoredObjPool;
        typedef std::list<int, monitoredObjPool >                       monitoredList;

        monitoredPool       pool(4096, 1, stdAllocator);
        monitoredAllocator  allocator(pool);
        monitoredObjPool    objPool(200, allocator);        //obj pool for 100 list nodes of integers
        monitoredList       list(objPool);

        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], (size_t)1, "alloc count failed", __FILE__, __LINE__, __FUNCTION__);
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], (size_t)4096, "alloc bytes failed", __FILE__, __LINE__, __FUNCTION__);


        // inserting integers
        for (size_t i = 0; i < 200; ++i)
            list.push_back(i);

        // should not trigger any call to new, there is plenty of space in the first block of the pool
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], (size_t)2, "alloc count failed", __FILE__, __LINE__, __FUNCTION__);

        size_t expectedBytes = 4096 + 200 * 24 + 4 * 4; //base alloc + 200 list nodes + 4 integers to fit t_bigBlock struct
        TestHelpers::assertEqual(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], expectedBytes, "alloc bytes failed", __FILE__, __LINE__, __FUNCTION__);


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
        
    return (testNumber);
}