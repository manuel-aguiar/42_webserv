/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:01:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 13:32:10 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MPool_FixedElem.hpp"
#include "../PoolAllocator_FixedElem.hpp"

#include "../../../List/List.hpp"
#include <list>

#include "../../../_Tests/test.h"

/*
	Helper class to count the number of allocations and deallocations, via inheritance
*/
# include "AllocCounters.tpp"

int TestPart1(int testNumber)
{
	std::cout << "TEST " << testNumber++ << ": ";
	
	try
	{
		size_t counters[3];
		size_t expectedAllocCount = 0;

		std::memset(counters, 0, sizeof(counters));
		
		std::list<int, AllocCounter<int> > list1(AllocCounter<int>(10, counters));

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
			throw std::runtime_error("alloc/dealloc count mismatch");
		if (counters[0] != expectedAllocCount)
			throw std::runtime_error("alloc count should be 15");

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}

	std::cout << "TEST " << testNumber++ << ": ";
	
	try
	{
		size_t counters[3];
		size_t expectedAllocCount = 0;
		const size_t loopTimes = 100;

		std::memset(counters, 0, sizeof(counters));
		
		MPool_FixedElem<int> alloc(2000);

		PoolAllocCounter<int> poolAlloc(alloc, counters);

		//sharing the same pool
		std::list<int, PoolAllocCounter<int> > list1(poolAlloc);
		std::list<int, PoolAllocCounter<int> > list2(poolAlloc);

		for (size_t i = 0; i < loopTimes; ++i)
		{
			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;

			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;

			list1.pop_back();
			list2.pop_back();

			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;

			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;

			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;

			list1.pop_back();
			list2.pop_back();
		}

		list1.clear();
		list2.clear();

		if (counters[0] != counters[1])
			throw std::runtime_error("alloc/dealloc count mismatch");
		if (counters[0] != expectedAllocCount)
			throw std::runtime_error("alloc count doesn't match");

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
	
	return (testNumber);
}
