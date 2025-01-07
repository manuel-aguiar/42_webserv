/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:12:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 16:42:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>

// Project headers
# include "../Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "NginxMemPool_TestCounter.hpp"

# include "../../../_Tests/test.h"

int TestPart2(int testNumber)
{
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

		struct AllocationStats result = (struct AllocationStats) {};
		struct AllocationStats expected = (struct AllocationStats) {};

        //Nginx_MemoryPool replaced by a descendent with counters
        NginxMemPool_TestCounter pool(4096, 1, &result);
        
        if (result.m_allocCount != expected.m_allocCount)
            throw std::runtime_error("alloc count dont match, got " + to_string(result.m_allocCount) + " expected " + to_string(expected.m_allocCount)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (result.m_allocBytes != expected.m_allocBytes)
            throw std::runtime_error("alloc bytes dont match, got " + to_string(result.m_allocBytes) + " expected " + to_string(expected.m_allocBytes)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        void* ptr = pool.allocate(100);
        (void)ptr;

        expected.m_allocCount++;
        expected.m_allocBytes += 100;

        if (result.m_allocCount != expected.m_allocCount)
            throw std::runtime_error("alloc count dont match, got " + to_string(result.m_allocCount) + " expected " + to_string(expected.m_allocCount)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (result.m_allocBytes != expected.m_allocBytes)
            throw std::runtime_error("alloc bytes dont match, got " + to_string(result.m_allocBytes) + " expected " + to_string(expected.m_allocBytes)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

		struct AllocationStats result = (struct AllocationStats) {};
		struct AllocationStats expected = (struct AllocationStats) {};

        const int allocSize = 100;

        NginxMemPool_TestCounter pool(4096, 1, &result);
        
        if (result.m_allocCount != expected.m_allocCount)
            throw std::runtime_error("alloc count dont match, got " + to_string(result.m_allocCount) + " expected " + to_string(expected.m_allocCount)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (result.m_allocBytes != expected.m_allocBytes)
            throw std::runtime_error("alloc bytes dont match, got " + to_string(result.m_allocBytes) + " expected " + to_string(expected.m_allocBytes)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        
        for (size_t i = 0; i < 100; ++i)
        {
            void* ptr = pool.allocate(allocSize);
            (void)ptr;

            expected.m_allocCount++;
            expected.m_allocBytes += allocSize;
        }

        if (result.m_allocCount != expected.m_allocCount)
            throw std::runtime_error("alloc count dont match, got " + to_string(result.m_allocCount) + " expected " + to_string(expected.m_allocCount)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (result.m_allocBytes != expected.m_allocBytes)
            throw std::runtime_error("alloc bytes dont match, got " + to_string(result.m_allocBytes) + " expected " + to_string(expected.m_allocBytes)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}    
