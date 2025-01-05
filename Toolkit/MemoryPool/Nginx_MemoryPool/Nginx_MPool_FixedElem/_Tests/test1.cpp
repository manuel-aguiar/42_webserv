/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/05 22:06:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers

# include "../Nginx_MPool_FixedElem.hpp"
# include "../Nginx_PoolAllocator_FixedElem.hpp"

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
# include <list>



int TestPart1(int testNumber)
{
	std::cout << "TEST " << testNumber++ << ": ";
	
	try
	{

		const size_t								loopTimes = 100;
		const size_t								poolsize = 100;
		const size_t 								nodeSize = (sizeof(int) + 4 + 8 + 8); // int + 4 padding + 8 + 8 byte ptrs next and prev

		Nginx_MemoryPool 							pool(4096, 1);

		size_t 										firstElement = (size_t)pool.allocate(sizeof(size_t)) + sizeof(size_t);
		Nginx_MPool_FixedElem<int> 					alloc(pool, poolsize);
		std::list<int, Nginx_MPool_FixedElem<int> > list1(alloc);
		list1.push_back(0);
		size_t 										lastElement = (size_t)pool.allocate(sizeof(size_t));


		if (lastElement - firstElement != poolsize * nodeSize)
		{
			std::cout << "expected: " << poolsize * nodeSize <<", got: " << lastElement - firstElement << ", "
			<< (unsigned char *)firstElement << ", " << (unsigned char *)lastElement << std::endl;
			throw std::runtime_error("element size is not correct");
		}

		for (size_t i = 1; i < loopTimes; i++)
			list1.push_back(i);
		
		for (std::list<int, Nginx_MPool_FixedElem<int> >::iterator it = list1.begin(); it != list1.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			if (!(nodeAddress >= firstElement && nodeAddress < lastElement))
			{
				std::cout << "element at " << (void *)nodeAddress << " is outside range (" << (void *)firstElement << ", " << (void *)lastElement << ")" << std::endl;
				throw std::runtime_error("node address is out of bounds");
			}
		}


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

		const size_t								loopTimes = 100;
		const size_t								poolsize = 100;
		const size_t 								nodeSize = (sizeof(int) + 4 + 8 + 8); // int + 4 padding + 8 + 8 byte ptrs next and prev


		/*
			THIS TEST ONLY WORKS BECAUSE I AM FORCING EVERYTHING TO BE ALLOCATED
			IN THE SAME POOL BLOCK!!!!!!!!!!!!!!!!!!!!!!
		*/
		Nginx_MemoryPool 							pool(10000, 1);


		// one list
		size_t 										firstElement1 = (size_t)pool.allocate(sizeof(size_t)) + sizeof(size_t);
		Nginx_MPool_FixedElem<int> 					alloc1(pool, poolsize);
		std::list<int, Nginx_MPool_FixedElem<int> > list1(alloc1);

		// The fixed block only gets allocated when the first element is pushed
		list1.push_back(0);
		size_t 										lastElement1 = (size_t)pool.allocate(sizeof(size_t));


		// separate list
		size_t 										firstElement2 = (size_t)pool.allocate(sizeof(size_t)) + sizeof(size_t);
		Nginx_MPool_FixedElem<int> 					alloc2(pool, poolsize);
		std::list<int, Nginx_MPool_FixedElem<int> > list2(alloc2);

		// The fixed block only gets allocated when the first element is pushed
		list2.push_back(0);
		size_t 										lastElement2 = (size_t)pool.allocate(sizeof(size_t));



		//pushing to both repeatedly
		for (size_t i = 1; i < loopTimes; i++)
		{
			list1.push_back(i);
			list2.push_back(i);
		}
		/////////////////
		// pool blocks //
		/////////////////
		
		// List 1 must be allcoated in a block of this size
		if (lastElement1 - firstElement1 != poolsize * nodeSize)
		{
			std::cout << "expected: " << poolsize * nodeSize <<", got: " << lastElement1 - firstElement1 << ", "
			<< (unsigned char *)firstElement1 << ", " << (unsigned char *)lastElement1 << std::endl;
			throw std::runtime_error("element size is not correct");
		}

		// List2 must be allcoated in a block of this size
		if (lastElement2 - firstElement2 != poolsize * nodeSize)
		{
			std::cout << "expected: " << poolsize * nodeSize <<", got: " << lastElement2 - firstElement2 << ", "
			<< (unsigned char *)firstElement2 << ", " << (unsigned char *)lastElement2 << std::endl;
			throw std::runtime_error("element size is not correct");
		}

		// blocks don't overlap
		if (lastElement1 > firstElement2)
		{
			std::cout << "blocks overlap" << std::endl;
			throw std::runtime_error("blocks overlap");
		}

		/////////////////////
		//Elements in block//
		/////////////////////

		// list1 must be allocated in alloc1
		for (std::list<int, Nginx_MPool_FixedElem<int> >::iterator it = list1.begin(); it != list1.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			if (!(nodeAddress >= firstElement1 && nodeAddress < lastElement1))
			{
				std::cout << "element at " << (void *)nodeAddress << " is outside range (" << (void *)firstElement1 << ", " << (void *)lastElement1 << ")" << std::endl;
				throw std::runtime_error("node address is out of bounds");
			}
		}



		// list 2 must be allocated in alloc2
		for (std::list<int, Nginx_MPool_FixedElem<int> >::iterator it = list2.begin(); it != list2.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			if (!(nodeAddress >= firstElement2 && nodeAddress < lastElement2))
			{
				std::cout << "element at " << (void *)nodeAddress << " is outside range (" << (void *)firstElement2 << ", " << (void *)lastElement2 << ")" << std::endl;
				throw std::runtime_error("node address is out of bounds");
			}
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

		const size_t								loopTimes = 100;
		const size_t								poolsize = 200;
		const size_t 								nodeSize = (sizeof(int) + 4 + 8 + 8); // int + 4 padding + 8 + 8 byte ptrs next and prev


		/*
			THIS TEST ONLY WORKS BECAUSE I AM FORCING EVERYTHING TO BE ALLOCATED
			IN THE SAME POOL BLOCK!!!!!!!!!!!!!!!!!!!!!!
		*/
	
		Nginx_MemoryPool 										pool(10000, 1);
		
		Nginx_MPool_FixedElem<int> 								alloc(pool, poolsize);
		std::list<int, Nginx_MPool_FixedElem<int> >				list1(alloc);
		std::list<int, Nginx_MPool_FixedElem<int> >				list2(alloc);


		// The fixed block only gets allocated when the first element is pushed
		size_t 										firstElement = (size_t)pool.allocate(sizeof(size_t)) + sizeof(size_t);
		list1.push_back(0);
		size_t 										lastElement = (size_t)pool.allocate(sizeof(size_t));



		//pushing to both repeatedly
		for (size_t i = 1; i < loopTimes; i++)
		{
			list1.push_back(i);
			list2.push_back(i);
		}
		/////////////////
		// pool blocks //
		/////////////////
		
		// List 1 must be allcoated in a block of this size
		if (lastElement - firstElement != poolsize * nodeSize)
		{
			std::cout << "expected: " << poolsize * nodeSize <<", got: " << lastElement - firstElement << ", "
			<< (unsigned char *)firstElement << ", " << (unsigned char *)lastElement << std::endl;
			throw std::runtime_error("element size is not correct");
		}

		/////////////////////
		//Elements in block//
		/////////////////////

		// list1 must be allocated in the same block as list2
		for (std::list<int, Nginx_MPool_FixedElem<int> >::iterator it = list1.begin(); it != list1.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			if (!(nodeAddress >= firstElement && nodeAddress < lastElement))
			{
				std::cout << "element at " << (void *)nodeAddress << " is outside range (" << (void *)firstElement << ", " << (void *)lastElement << ")" << std::endl;
				throw std::runtime_error("node address is out of bounds");
			}
		}

		// list 2 must be allocated in the same block as list1
		for (std::list<int, Nginx_MPool_FixedElem<int> >::iterator it = list2.begin(); it != list2.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			if (!(nodeAddress >= firstElement && nodeAddress < lastElement))
			{
				std::cout << "element at " << (void *)nodeAddress << " is outside range (" << (void *)firstElement << ", " << (void *)lastElement << ")" << std::endl;
				throw std::runtime_error("node address is out of bounds");
			}
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