/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 09:24:52 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 11:52:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
#include "../../MemoryPool/MemoryPool.h"
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"
#include "../../_Tests/test.h"

// C++ headers
#include <list>
#include <iostream>
#include <cstring>
#include <stdexcept>


int TestPart3(int testNumber)
{
	Nginx_MemoryPool* memoryPool = Nginx_MemoryPool::create(4096);
	
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		Nginx_MPool_FixedElem<int> fixedElem(memoryPool, 300);
		Nginx_PoolAllocator_FixedElem<int> alloc(fixedElem);
		
		List<int, Nginx_PoolAllocator_FixedElem<int> > first(alloc);
		List<int, Nginx_PoolAllocator_FixedElem<int> > second(alloc);


		for (int i = 0; i < 100; ++i)
		{
			first.push_back(i);
			second.emplace_back(i);
		}

		if (first.size() != second.size())
			throw std::logic_error("size mismatch");
		
		if (first.getAllocator() != second.getAllocator())
			throw std::logic_error("allocator mismatch");

		List<int, Nginx_PoolAllocator_FixedElem<int> >::iterator it = second.begin();
		List<int, Nginx_PoolAllocator_FixedElem<int> >::iterator iter = first.begin();
		for ( ; it != second.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("same pointer");
		}


		// allocates on the same memory pool
		List<int, Nginx_PoolAllocator_FixedElem<int> > copy(second);

		it = copy.begin();
		iter = first.begin();
		for ( ; it != copy.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy constructor, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy constructor, same pointer");

		}
		if (copy.getAllocator() != first.getAllocator())
			throw std::logic_error("allocator mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		Nginx_MPool_FixedElem<int> fixedElem(memoryPool, 300);
		Nginx_PoolAllocator_FixedElem<int> alloc(fixedElem);
		
		List<int, Nginx_PoolAllocator_FixedElem<int> > first(alloc);
		List<int, Nginx_PoolAllocator_FixedElem<int> > second(alloc);


		for (int i = 0; i < 100; ++i)
		{
			first.push_back(i);
			second.emplace_back(i);
		}

		if (first.size() != second.size())
			throw std::logic_error("size mismatch");
		
		if (first.getAllocator() != second.getAllocator())
			throw std::logic_error("allocator mismatch");

		List<int, Nginx_PoolAllocator_FixedElem<int> >::iterator it = second.begin();
		List<int, Nginx_PoolAllocator_FixedElem<int> >::iterator iter = first.begin();
		for ( ; it != second.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("same pointer");
		}


		
		//Copy starts by alllocating somewhere else
		Nginx_MPool_FixedElem<int> fixedElem2(memoryPool, 100);
		Nginx_PoolAllocator_FixedElem<int> alloc2(fixedElem2);

		
		List<int, Nginx_PoolAllocator_FixedElem<int> > copy(alloc2);

		for (size_t i = 0; i < second.size() / 2; ++i)
			copy.emplace_back(i);

		// copy should allocate on the same memory pool as first
		copy = first;

		it = copy.begin();
		iter = first.begin();
		for ( ; it != copy.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy assignment, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy assignment, same pointer");

		}
		if (copy.getAllocator() != first.getAllocator())
			throw std::logic_error("copy assignment, allocator mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

	memoryPool->destroy();

	return (testNumber);
}

int TestPart2(int testNumber)
{
	Nginx_MemoryPool* memoryPool = Nginx_MemoryPool::create(4096);

	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		List<int> list;

		for (int i = 0; i < 100; ++i)
			list.emplace_back(i);

		List<int> copy(list);
		List<int>::iterator iter = copy.begin();
		List<int>::iterator it = list.begin();

		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy constructor, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy constructor, same pointer");
		}

		if (list.size() != copy.size())
			throw std::logic_error("copy constructor, size mismatch");

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}
	

	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		List<int> list;

		for (int i = 0; i < 100; ++i)
			list.emplace_back(i);

		List<int> copy;

		copy = list;
		List<int>::iterator iter = copy.begin();
		List<int>::iterator it = list.begin();

		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy assignment, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy constructor, same pointer");
		}

		if (list.size() != copy.size())
			throw std::logic_error("copy assignment, size mismatch");

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}


	
	

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		Nginx_PoolAllocator<ToolkitDummy> alloc(memoryPool);
		std::list<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > std(alloc);
		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > list(alloc);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator it = list.begin();
		std::list<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
			if (&(*it) == &(*iter))
				throw std::logic_error("copy constructor, same pointer");
		}

		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > copy(list);

		it = copy.begin();
		iter = std.begin();
		for ( ; it != copy.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy constructor, value mismatch");
			if (&(*it) == &(*iter))
				throw std::logic_error("copy constructor, same pointer");
		}


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		Nginx_MPool_FixedElem<ToolkitDummy> fixedElem(memoryPool, 200);

		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> > first(fixedElem);
		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> > second(fixedElem);

		for (int i = 0; i < 100; ++i)
		{
			first.push_back(i);
			second.emplace_back(i);
		}
		if (first.size() != second.size())
			throw std::logic_error("size mismatch");
		
		if (first.getAllocator() != second.getAllocator())
			throw std::logic_error("allocator mismatch");

		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> >::iterator it = second.begin();
		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> >::iterator iter = first.begin();
		for ( ; it != second.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("same pointer");
		}

		List<ToolkitDummy, Nginx_MPool_FixedElem<ToolkitDummy> > copy(second); /// should allocate on the same memory pool

		it = copy.begin();
		iter = first.begin();
		for ( ; it != copy.end() && iter != first.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("copy constructor, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy constructor, same pointer");

		}
		if (copy.getAllocator() != first.getAllocator())
			throw std::logic_error("allocator mismatch");

		std::cout << "	PASSED" << std::endl;
	}

	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

	memoryPool->destroy();
	

	return (testNumber);
}