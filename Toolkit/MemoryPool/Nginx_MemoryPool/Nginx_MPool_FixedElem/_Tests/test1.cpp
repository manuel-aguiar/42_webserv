/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/06 11:35:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers

# include "../Nginx_MPool_FixedElem.hpp"

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

# include "../Nginx_Shared_FixedElem.hpp"

int TestPart1(int testNumber)
{
	
	{
		Nginx_MemoryPool 							pool(4096, 1);
		Nginx_MPool_FixedElem<int> 					alloc(pool, 0);


		std::list<int, Nginx_Shared_FixedElem<int> > list1((Nginx_Shared_FixedElem<int>(alloc)));

		alloc.resize(1);

		list1.push_back(0);
	}

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
			throw std::runtime_error("element size is not correct, got: " + to_string( lastElement - firstElement) 
			+ " expected: " + to_string( poolsize * nodeSize ) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

		for (size_t i = 1; i < loopTimes; i++)
			list1.push_back(i);
		
		for (std::list<int, Nginx_MPool_FixedElem<int> >::iterator it = list1.begin(); it != list1.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			if (!(nodeAddress >= firstElement && nodeAddress < lastElement))
			{
				throw std::runtime_error("element  " + to_string((void *)nodeAddress) + " is outside range(" +
				to_string((void *)firstElement) + ", " + to_string((void *)lastElement) + ")\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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

			otherwise these pointer calculations don't work, just that.
			The FixedPools would work just fine, but ask nginx for more memory which internally
			to Nginx_MemoryPool would mean more than a block

			But we asked Ngin_MemoryPool to be created with a big block, so it fits :)
		*/

		Nginx_MemoryPool 							pool(10000, 1);


		// one list
		size_t 										firstElement1 = (size_t)pool.allocate(sizeof(size_t)) + sizeof(size_t);
		std::list<int, Nginx_MPool_FixedElem<int> > list1(Nginx_MPool_FixedElem<int>(pool, poolsize));

		// The fixed block only gets allocated when the first element is pushed
		list1.push_back(0);
		size_t 										lastElement1 = (size_t)pool.allocate(sizeof(size_t));


		// separate list
		size_t 										firstElement2 = (size_t)pool.allocate(sizeof(size_t)) + sizeof(size_t);
		std::list<int, Nginx_MPool_FixedElem<int> > list2(Nginx_MPool_FixedElem<int>(pool, poolsize));

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
			throw std::runtime_error("element size is not correct, got: " + to_string( lastElement1 - firstElement1) 
			+ " expected: " + to_string( poolsize * nodeSize ) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

		// List2 must be allcoated in a block of this size
		if (lastElement2 - firstElement2 != poolsize * nodeSize)
		{
			throw std::runtime_error("element size is not correct, got: " + to_string( lastElement2 - firstElement2) 
			+ " expected: " + to_string( poolsize * nodeSize ) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

		// blocks don't overlap
		if (lastElement1 > firstElement2)
		{
			throw std::runtime_error(std::string("blocks are overlapping: \n")
			+ "\tfirst block: (" + to_string((void *)firstElement1) + ", " + to_string((void *)lastElement1) + ")\n"
			+ "\tsecnd block: (" + to_string((void *)firstElement2) + ", " + to_string((void *)lastElement2) + ")\n"
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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
				throw std::runtime_error("element  " + to_string((void *)nodeAddress) + " is outside range(" +
				to_string((void *)firstElement1) + ", " + to_string((void *)lastElement1) + ")\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
			}
		}



		// list 2 must be allocated in alloc2
		for (std::list<int, Nginx_MPool_FixedElem<int> >::iterator it = list2.begin(); it != list2.end(); ++it)
		{
			size_t nodeAddress;
			nodeAddress = (size_t)&(*it);
			if (!(nodeAddress >= firstElement2 && nodeAddress < lastElement2))
			{
				throw std::runtime_error("element  " + to_string((void *)nodeAddress) + " is outside range(" +
				to_string((void *)firstElement2) + ", " + to_string((void *)lastElement2) + ")\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
			}
		}


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}