/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:12:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 00:14:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>
# include <list>

// Project headers
# include "../Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "NginxMemPool_TestCounter.hpp"
# include "NginxAllocator_TestCounter.tpp"

# include "../../../_Tests/test.h"

int TestPart3(int testNumber)
{
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

		struct AllocationStats result = (struct AllocationStats) {};
		struct AllocationStats expected = (struct AllocationStats) {};

        const int nodeSize = 24;

        Nginx_MemoryPool pool(4096, 1);
        NginxAllocator_TestCounter<int> alloc(pool, &result);
        std::list<int, NginxAllocator_TestCounter<int> > list1(alloc);
        
        if (result.m_allocCount != expected.m_allocCount)
            throw std::runtime_error("alloc count dont match, got " + to_string(result.m_allocCount) + " expected " + to_string(expected.m_allocCount)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (result.m_allocBytes != expected.m_allocBytes)
            throw std::runtime_error("alloc bytes dont match, got " + to_string(result.m_allocBytes) + " expected " + to_string(expected.m_allocBytes)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        for (size_t i = 0; i < 100; i++)
        {
            list1.push_back(i);
            expected.m_allocCount++;
            expected.m_allocBytes += nodeSize;
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

/****************************************** */

    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

		struct AllocationStats result = (struct AllocationStats) {};
		struct AllocationStats expected = (struct AllocationStats) {};

        const int nodeSize = 48;

        Nginx_MemoryPool pool(4096, 1);

        //original pool, with the Nginx_PoolAllocator replaced by a descendent with counters
        NginxAllocator_TestCounter<std::string> alloc(pool, &result);
        
        std::list<std::string, NginxAllocator_TestCounter<std::string> > list1(alloc);
        
        if (result.m_allocCount != expected.m_allocCount)
            throw std::runtime_error("alloc count dont match, got " + to_string(result.m_allocCount) + " expected " + to_string(expected.m_allocCount)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (result.m_allocBytes != expected.m_allocBytes)
            throw std::runtime_error("alloc bytes dont match, got " + to_string(result.m_allocBytes) + " expected " + to_string(expected.m_allocBytes)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        for (size_t i = 0; i < 100; i++)
        {
            list1.push_back(" a big string that allocates on the heap plsssssss" + to_string(i));
            expected.m_allocCount++;
            expected.m_allocBytes += nodeSize;
        }

        if (result.m_allocCount != expected.m_allocCount)
            throw std::runtime_error("alloc count dont match, got " + to_string(result.m_allocCount) + " expected " + to_string(expected.m_allocCount)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (result.m_allocBytes != expected.m_allocBytes)
            throw std::runtime_error("alloc bytes dont match, got " + to_string(result.m_allocBytes) + " expected " + to_string(expected.m_allocBytes)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        //integrity test

        for (size_t i = 0; i < 100; i++)
        {
            if (list1.front() != " a big string that allocates on the heap plsssssss" + to_string(i))
                throw std::runtime_error("string mismatch " + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
            list1.pop_front();
        }

        //Should lead to no leaks, destructor reverse order. list clears std::strings and their internal mallocs,
        // then allocator vanishes, and finally pool deletes blocks
        
        //All RAII

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

/****************************************** */

    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

		struct AllocationStats result = (struct AllocationStats) {};
		struct AllocationStats expected = (struct AllocationStats) {};

        const int nodeSize = 48;

        Nginx_MemoryPool pool(4096, 1);

        //original pool, with the Nginx_PoolAllocator replaced by a descendent with counters
        NginxAllocator_TestCounter<std::string> allocStrings(pool, &result);
        NginxAllocator_TestCounter<int>         allocInts(pool, &result);

        // two lists sharing the same memorypool
        std::list<std::string, NginxAllocator_TestCounter<std::string> >    list1(allocStrings);
        std::list<int, NginxAllocator_TestCounter<int> >                    list2(allocInts);

        if (result.m_allocCount != expected.m_allocCount)
            throw std::runtime_error("alloc count dont match, got " + to_string(result.m_allocCount) + " expected " + to_string(expected.m_allocCount)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (result.m_allocBytes != expected.m_allocBytes)
            throw std::runtime_error("alloc bytes dont match, got " + to_string(result.m_allocBytes) + " expected " + to_string(expected.m_allocBytes)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        for (size_t i = 0; i < 100; i++)
        {
            list1.push_back(" a big string that allocates on the heap plsssssss" + to_string(i));
            expected.m_allocCount++;
            expected.m_allocBytes += nodeSize;

            list2.push_back(i);
            expected.m_allocCount++;
            expected.m_allocBytes += 24; //4 int + 4 padding + 8 next + 8 prev
        }

        if (result.m_allocCount != expected.m_allocCount)
            throw std::runtime_error("alloc count dont match, got " + to_string(result.m_allocCount) + " expected " + to_string(expected.m_allocCount)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        
        if (result.m_allocBytes != expected.m_allocBytes)
            throw std::runtime_error("alloc bytes dont match, got " + to_string(result.m_allocBytes) + " expected " + to_string(expected.m_allocBytes)
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        //integrity test
        for (size_t i = 0; i < 100; i++)
        {
            if (list1.front() != " a big string that allocates on the heap plsssssss" + to_string(i))
                throw std::runtime_error("string mismatch " + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
            list1.pop_front();
            if (list2.front() != (int)i)
                throw std::runtime_error("int mismatch " + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
            list2.pop_front();
        }

        //Should lead to no leaks, destructor reverse order. list clears std::strings and their internal mallocs,
        // then allocator vanishes, and finally pool deletes blocks
        
        //All RAII

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}    
