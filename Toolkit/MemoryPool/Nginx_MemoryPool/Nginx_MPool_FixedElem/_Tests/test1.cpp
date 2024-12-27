/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 16:49:12 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers

# include "../Nginx_MPool_FixedElem.hpp"
# include "../Nginx_PoolAllocator_FixedElem.hpp"

# include "../../Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator.hpp"
# include "../../_Tests/NginxAllocCounters.tpp"
# include "../../../../_Tests/test.h"

// C++ headers
# include <climits>
# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <cassert>
# include <cstring>
# include <list>



int TestPart1(int testNumber)
{
	std::cout << "TEST " << testNumber++ << ": ";
	
	try
	{

		size_t totalAllocs = 0;
		size_t totalBytes = 0;

		size_t expectedAllocCount = 0;
		size_t expectedBytes = 0;

		const size_t loopTimes = 100;
		const size_t poolsize = 100;

		Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096, 1);

		Nginx_MPool_FixedElem<int> alloc(pool, poolsize);

		std::list<int, Nginx_MPool_FixedElem<int> > list1(alloc);

		const size_t nodeSize = (sizeof(int) + 4 + 8 + 8); // int + 4 padding + 8 + 8 byte ptrs next and prev

		for (size_t i = 0; i < loopTimes; i++)
			list1.push_back(i);

		list1.clear();
		
		expectedAllocCount = 1; // just for the block asked for in the FixedElem, from the nginx pool
		expectedBytes = poolsize * nodeSize; // total nodes asked for at once by the FixedElem

		pool->destroy();

		if (totalBytes != expectedBytes)
		{
			std::cout << "expected bytes: " << expectedBytes << " got: " << totalBytes << std::endl;
			throw std::runtime_error("total bytes dont match");
		}
		if (totalAllocs != expectedAllocCount)
		{
			std::cout << "expected allocs: " << expectedAllocCount << " got: " << totalAllocs << std::endl;
			throw std::runtime_error("alloc count should be 1");
		}

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}

	return (testNumber);	
}