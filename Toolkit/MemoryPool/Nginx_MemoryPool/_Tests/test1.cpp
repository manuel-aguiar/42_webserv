

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>
# include <list>

// Project headers
# include "../Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../Heap_ObjectPool/Heap_ObjectPool.hpp"
# include "../../../TestHelpers/TestHelpers.h"
# include "TestAllocator.tpp"

int TestPart1(int testNumber)
{
    // instantiation
    try
    {
        TEST_INTRO(testNumber++);
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

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    // instantiation
    try
    {
        TEST_INTRO(testNumber++);

        size_t counters[TestAllocator<t_byte>::E_TOTAL_COUNTERS] = {0};

        // inspecting the internals of the Nginx_MemoryPool by infusing an allocator that counts
        TestAllocator<t_byte> alloc(counters);
        Impl_Nginx_MemoryPool<t_byte, TestAllocator<t_byte> > pool(4096, 1, alloc);

        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], 1, "alloc count failed");
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], 4096, "alloc bytes failed");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}


    // instantiation
    try
    {
        TEST_INTRO(testNumber++);

        size_t counters[TestAllocator<t_byte>::E_TOTAL_COUNTERS] = {0};

        // dark magic, using Nginx_MemoryPool to allocate ints instead of bytes, don't do this at home
        TestAllocator<t_byte> alloc(counters);
        Impl_Nginx_MemoryPool<int, TestAllocator<t_byte> > pool(100, 1, alloc);

        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], 1, "alloc count failed");
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], (size_t)(100 * (sizeof(int))), "alloc bytes failed");

        void* bigBlock = pool.allocate(200);
        (void)bigBlock;

        //big block, will overwelm the pool

        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], 2, "alloc count failed");

        size_t expectedBytes = (100 + 200 + 16 / sizeof(int)) * (sizeof(int)); //16 / sizeof(int)(how many ints i have to allocate to have place for the big block struct)
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], expectedBytes, "alloc bytes failed");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    try     // using the memory pool to allocate memory for linked list nodes
    {
        TEST_INTRO(testNumber++);

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
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], 1, "alloc count failed");
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], 4096, "alloc bytes failed");


        // inserting integers
        for (size_t i = 0; i < 100; ++i)
            list.push_back(i);


        // should not trigger any call to new, there is plenty of space in the first block of the pool
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], 1, "alloc count failed");
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], 4096, "alloc bytes failed");


		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    try     // using the memory pool to allocate memory for linked list nodes
    {
        TEST_INTRO(testNumber++);

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

        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], 1, "alloc count failed");
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], 4096, "alloc bytes failed");


        // inserting integers
        for (size_t i = 0; i < 200; ++i)
            list.push_back(i);

        // should not trigger any call to new, there is plenty of space in the first block of the pool
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_COUNT], 2, "alloc count failed");

        size_t expectedBytes = 4096 + 200 * 24 + 4 * 4; //base alloc + 200 list nodes + 4 integers to fit t_bigBlock struct
        EXPECT_EQUAL(counters[TestAllocator<t_byte>::E_ALLOC_BYTES], expectedBytes, "alloc bytes failed");


		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
        
    return (testNumber);
}