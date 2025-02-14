

// Project headers
#include "../Heap_ObjectPool.hpp"
#include "../../../TestHelpers/TestHelpers.h"

// C++ headers
#include <map>
#include <list>


/*
	Helper class to count the number of allocations and deallocations, via inheritance
	It is used to test the Heap_ObjectPool class
*/
# include "FixedElem_AllocCounter.tpp"

int TestPart1(int testNumber)
{

	TEST_INTRO(testNumber++);
	
	try
	{
		size_t counters[3];
		size_t expectedAllocCount = 0;

		std::memset(counters, 0, sizeof(counters));
		
		std::list<int, FixedElem_AllocCounter<int> > list1(FixedElem_AllocCounter<int>(10, counters));

		list1.push_back(1);
		expectedAllocCount += 1;

		list1.push_back(2);
		expectedAllocCount += 1;

		list1.push_back(3);
		expectedAllocCount += 1;

		list1.push_back(1);
		expectedAllocCount += 1;

		list1.push_back(2);
		expectedAllocCount += 1;

		list1.push_back(3);
		expectedAllocCount += 1;

		list1.push_back(1);
		expectedAllocCount += 1;

		list1.push_back(2);
		expectedAllocCount += 1;

		list1.push_back(3);
		expectedAllocCount += 1;
   
		list1.push_back(3);
		expectedAllocCount += 1;
 
		list1.pop_front();
		list1.pop_front();
		list1.pop_front();
		list1.pop_front();
		list1.push_back(3);
		expectedAllocCount += 1;
		 
		list1.pop_front();
		list1.push_back(3);
		expectedAllocCount += 1;
		   
		list1.push_back(3);
		expectedAllocCount += 1;
		
		list1.pop_front();
		list1.pop_front();
		list1.pop_front();
		list1.push_back(3);
		expectedAllocCount += 1;
		   
		list1.push_back(3);
		expectedAllocCount += 1;

		list1.clear();

		EXPECT_EQUAL(counters[0], counters[1], "alloc/dealloc count mismatch");
		EXPECT_EQUAL(counters[0], expectedAllocCount, "alloc count failed");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}


	TEST_INTRO(testNumber++);
	
	try
	{
		size_t counters[3];
		size_t expectedAllocCount = 0;

		std::memset(counters, 0, sizeof(counters));
		
		// oh yeah, doesn't get more verbose than this
		std::map<int, int, std::less<int>, FixedElem_AllocCounter<std::pair<int, int> > > 
		map1(std::less<int>(), FixedElem_AllocCounter<std::pair<int, int> >(10, counters));

		map1[42] = 789;
		expectedAllocCount += 1;

		map1[43] = 345;
		expectedAllocCount += 1;

		map1[2] = 345;
		expectedAllocCount += 1;

		map1.clear();

		map1[3] = 123;
		expectedAllocCount += 1;

		map1.clear();

		EXPECT_EQUAL(counters[0], counters[1], "alloc/dealloc count mismatch");
		EXPECT_EQUAL(counters[0], expectedAllocCount, "alloc count failed");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
	
	return (testNumber);
}
