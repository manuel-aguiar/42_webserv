/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 14:02:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <climits>
# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <cassert>
# include "../Nginx_MemoryPool.hpp"
# include <cstring>

# include "../Nginx_MemoryPool.hpp"
# include "../Nginx_PoolAllocator.hpp"

#include "../../../_Tests/test.h"

// inherits, that's the one we will use to count
# include "NginxAllocCounters.tpp"


int TestPart1(int testNumber)
{
std::cout << "TEST " << testNumber++ << ": ";
	
	try
	{

		size_t counters[3];
		size_t totalBytes = 0;
		size_t expectedAllocCount = 0;
		size_t expectedBytes = 0;



		std::memset(counters, 0, sizeof(counters));
		
		NginxAllocCounter* pool = NginxAllocCounter::create(4096, counters, &totalBytes, 1);

		void *ptr = pool->allocate(100);
		(void)ptr;
		expectedAllocCount += 1;
		expectedBytes += 100;

		if (totalBytes != expectedBytes)
			throw std::runtime_error("total bytes should be 100");
		if (counters[0] != expectedAllocCount)
			throw std::runtime_error("alloc count should be 1");

		pool->destroy();

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
	
	return (testNumber);	
}