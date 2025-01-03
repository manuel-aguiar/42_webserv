/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 14:59:25 by mmaria-d         ###   ########.fr       */
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


/*
		
			Expected bytes are hard to track since, internally, the list will also allocate for its nodes
		
		(void)expectedBytes;

*/

int TestPart1(int testNumber)
{
	std::cout << "TEST " << testNumber++ << ": ";
	
	try
	{

		size_t totalAllocs = 0;
		size_t totalBytes = 0;

		size_t expectedAllocCount = 0;
		size_t expectedBytes = 0;

		const size_t allocSize = 100;
		
		NginxAllocCounter* pool = NginxAllocCounter::create(4096, &totalAllocs, &totalBytes, 1);

		for (size_t i = 0; i < 10; i++)
		{
			void *ptr = pool->allocate(allocSize);
			(void)ptr;
			
			expectedAllocCount += 1;
			expectedBytes += allocSize;
		}

		pool->destroy();

		if (totalBytes != expectedBytes)
			throw std::runtime_error("total bytes should be 100");
		if (totalAllocs != expectedAllocCount)
			throw std::runtime_error("alloc count should be 1");

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
		   

		size_t totalAllocs = 0;
		size_t totalBytes = 0;

		size_t expectedAllocCount = 0;
		size_t expectedBytes = 0;

		NginxAllocCounter* pool = NginxAllocCounter::create(4096, &totalAllocs, &totalBytes, 1);

		for (int i = 0; i < 100; i++)
		{
			std::string& aString = *(std::string *)pool->allocate((sizeof(std::string)), true);
			expectedAllocCount++;
			expectedBytes += sizeof(std::string);
			new (&aString) std::string("bananas"); //small no realloc
		}
		
		pool->allocate(10000, true); // big block
		pool->allocate(10000, true); // big block
		pool->allocate(10000, true); // big block
		pool->allocate(10000, true); // big block
		pool->allocate(10000, true); // big block
		expectedAllocCount += 5;
		expectedBytes += 5 * 10000;

		char *str = (char *)pool->allocate(sizeof("Hello World!"), false);
		(void)str;
		expectedAllocCount++;
		expectedBytes += sizeof("Hello World!");

		pool->destroy();

		if (totalBytes != expectedBytes)
			throw std::runtime_error("total bytes dont match");
		if (totalAllocs != expectedAllocCount)
			throw std::runtime_error("alloc count dont match");

		
		
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
		   

		size_t totalAllocs = 0;
		size_t totalBytes = 0;

		size_t expectedAllocCount = 0;
		size_t expectedBytes = 0;

		NginxAllocCounter* pool = NginxAllocCounter::create(4096, &totalAllocs, &totalBytes, 1);

		for (int i = 0; i < 100; i++)
		{
			std::string& aString = *(std::string *)pool->allocate((sizeof(std::string)), true);
			expectedAllocCount++;
			expectedBytes += sizeof(std::string);
			new (&aString) std::string("bananas"); //small no realloc
		}
		

		pool->destroy();

		if (totalBytes != expectedBytes)
			throw std::runtime_error("total bytes dont match");
		if (totalAllocs != expectedAllocCount)
			throw std::runtime_error("alloc count dont match");
		
		
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}	

	return (testNumber);	
}