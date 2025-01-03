/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 17:07:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../../Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator.hpp"
# include "../../../../_Tests/test.h"

// C++ headers
# include <climits>
# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <cassert>
# include <cstring>


int TestPart2(int testNumber)
{
	/*
	std::cout << "TEST " << testNumber++ << ": ";
	
	try
	{
		size_t counters[3];
		
		size_t totalBytes = 0;

		size_t expectedAllocCount = 0;
		size_t expectedBytes = 0;

		const size_t loopTimes = 100;

		std::memset(counters, 0, sizeof(counters));
		
		Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096, 1);

		NginxPoolAllocatorCounter<int> alloc(pool, counters, &totalBytes);

		//sharing the same pool
		std::list<int, NginxPoolAllocatorCounter<int> > list1(alloc);
		std::list<int, NginxPoolAllocatorCounter<int> > list2(alloc);

		const size_t nodeSize = (sizeof(int) + 4 + 8 + 8); // int + 4 padding + 8 + 8 byte ptrs next and prev

		for (size_t i = 0; i < loopTimes; ++i)
		{
			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;
			expectedBytes += (nodeSize) * 2;

			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;
			expectedBytes += (nodeSize) * 2;

			list1.pop_back();
			list2.pop_back();

			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;
			expectedBytes += (nodeSize) * 2;

			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;
			expectedBytes += (nodeSize) * 2;

			list1.push_back(i);
			list2.push_back(i);
			expectedAllocCount += 2;
			expectedBytes += (nodeSize) * 2;

			list1.pop_back();
			list2.pop_back();
		}

		list1.clear();
		list2.clear();

		pool->destroy();

		if (counters[0] != counters[1])
		{
			std::cout << "expected allocs: " << counters[0] << " deallocs: " << counters[1] << std::endl;
			throw std::runtime_error("alloc/dealloc count mismatch");
		}
			
		if (counters[0] != expectedAllocCount)
		{
			std::cout << "expected allocs: " << expectedAllocCount << " got: " << counters[0] << std::endl;
			throw std::runtime_error("alloc count doesn't match");
		}
		if (totalBytes != expectedBytes)
		{
			std::cout << "expected bytes: " << expectedBytes << " got: " << totalBytes << std::endl;
			throw std::runtime_error("total bytes dont match");
		}
		
		

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
*/
	return (testNumber);	
}