

// Project headers
#include "../Heap_ObjectPool.hpp"
#include "../../../_Tests/TestHelpers.h"

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
	std::cout << "TEST " << testNumber++ << ": ";
	
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

		if (counters[0] != counters[1])
			throw std::runtime_error("alloc/dealloc count mismatch, allocs: " + TestHelpers::to_string(counters[0]) 
			+ " deallocs: " + TestHelpers::to_string(counters[1]) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		if (counters[0] != expectedAllocCount)
			throw std::runtime_error("alloc count failed, got " + TestHelpers::to_string(counters[0]) 
			+ " expected: " + TestHelpers::to_string(expectedAllocCount) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


	std::cout << "TEST " << testNumber++ << ": ";
	
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

		if (counters[0] != counters[1])
			throw std::runtime_error("alloc/dealloc count mismatch, allocs: " + TestHelpers::to_string(counters[0]) 
			+ " deallocs: " + TestHelpers::to_string(counters[1]) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		if (counters[0] != expectedAllocCount)
			throw std::runtime_error("alloc count failed, got " + TestHelpers::to_string(counters[0]) 
			+ " expected: " + TestHelpers::to_string(expectedAllocCount) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
	
	return (testNumber);
}
