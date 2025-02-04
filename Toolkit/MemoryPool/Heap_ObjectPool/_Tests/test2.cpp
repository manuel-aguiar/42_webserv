

// Project headers

# include "../Heap_ObjectPool.hpp"

# include "../../Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../../TestHelpers/TestHelpers.h"

// C++ headers
# include <climits>
# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <cassert>
# include <cstring>
# include <list>



int TestPart2(int testNumber)
{
	TEST_INTRO(testNumber++);
	
	try
	{

		const size_t								loopTimes = 100;
		const size_t								poolsize = 100;
		const size_t 								nodeSize = (sizeof(int) + 4 + 8 + 8); // int + 4 padding + 8 + 8 byte ptrs next and prev

		Nginx_MemoryPool 							pool(4096, 1);

		size_t 										firstElement = (size_t)pool.allocate(sizeof(size_t)) + sizeof(size_t);
		Heap_ObjectPool<int, Nginx_PoolAllocator<int, Nginx_MemoryPool> >					
													alloc(poolsize, pool);
		
		std::list<int, Heap_ObjectPool<int, Nginx_PoolAllocator<int, Nginx_MemoryPool> > > 
													list1(alloc);
		
		list1.push_back(0);
		size_t 										lastElement = (size_t)pool.allocate(sizeof(size_t));

		EXPECT_EQUAL(lastElement - firstElement, poolsize * nodeSize, "element size is not correct");

		for (size_t i = 1; i < loopTimes; i++)
			list1.push_back(i);
		
		for (std::list<int, Heap_ObjectPool<int, Nginx_PoolAllocator<int, Nginx_MemoryPool> > >::iterator it = list1.begin(); it != list1.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			EXPECT_EQUAL(nodeAddress >= firstElement && nodeAddress < lastElement, true, "element is outside range");
		}


		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	TEST_INTRO(testNumber++);
	
	try
	{

		const size_t								loopTimes = 100;
		const size_t								poolsize = 100;
		const size_t 								nodeSize = (sizeof(int) + 4 + 8 + 8); // int + 4 padding + 8 + 8 byte ptrs next and prev


		/*
			THIS TEST ONLY WORKS BECAUSE I AM FORCING EVERYTHING TO BE ALLOCATED
			IN THE SAME POOL BLOCK!!!!!!!!!!!!!!!!!!!!!!

			otherwise these pointer calculations don't work, just that.
			The FixedPools would work just fine, but ask nginx for more memory which internally
			to Nginx_MemoryPool would mean more than a block

			But we asked Ngin_MemoryPool to be created with a big block, so it fits :)
		*/

		Nginx_MemoryPool 							pool(10000, 1);


		// one list
		size_t 										firstElement1 = (size_t)pool.allocate(sizeof(size_t)) + sizeof(size_t);
		std::list<int, Heap_ObjectPool<int, Nginx_PoolAllocator<int, Nginx_MemoryPool> > > 
													list1(Heap_ObjectPool<int, Nginx_PoolAllocator<int, Nginx_MemoryPool> >(poolsize, pool));

		// The fixed block only gets allocated when the first element is pushed
		list1.push_back(0);
		size_t 										lastElement1 = (size_t)pool.allocate(sizeof(size_t));


		// separate list
		size_t 										firstElement2 = (size_t)pool.allocate(sizeof(size_t)) + sizeof(size_t);
		std::list<int, Heap_ObjectPool<int, Nginx_PoolAllocator<int, Nginx_MemoryPool> > > 
													list2(Heap_ObjectPool<int, Nginx_PoolAllocator<int, Nginx_MemoryPool> >(poolsize, pool));

		// The fixed block only gets allocated when the first element is pushed
		list2.push_back(0);
		size_t 										lastElement2 = (size_t)pool.allocate(sizeof(size_t));



		//pushing to both repeatedly
		for (size_t i = 1; i < loopTimes; i++)
		{
			list1.push_back(i);
			list2.push_back(i);
		}
		/////////////////
		// pool blocks //
		/////////////////
		
		// List 1 must be allcoated in a block of this size
		EXPECT_EQUAL(lastElement1 - firstElement1, poolsize * nodeSize, "element size is not correct");

		// List2 must be allcoated in a block of this size
		EXPECT_EQUAL(lastElement2 - firstElement2, poolsize * nodeSize, "element size is not correct");

		// blocks don't overlap
		EXPECT_EQUAL(lastElement1 <= firstElement2, true, "blocks are overlapping");

		/////////////////////
		//Elements in block//
		/////////////////////

		// list1 must be allocated in alloc1
		for (std::list<int, Heap_ObjectPool<int, Nginx_PoolAllocator<int, Nginx_MemoryPool> > >::iterator it = list1.begin(); it != list1.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			EXPECT_EQUAL(nodeAddress >= firstElement1 && nodeAddress < lastElement1, true, "element is outside range");
		}


		// list 2 must be allocated in alloc2
		for (std::list<int, Heap_ObjectPool<int, Nginx_PoolAllocator<int, Nginx_MemoryPool> > >::iterator it = list2.begin(); it != list2.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			EXPECT_EQUAL(nodeAddress >= firstElement2 && nodeAddress < lastElement2, true, "element is outside range");
		}


		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	return (testNumber);
}