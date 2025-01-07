/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:12:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 23:51:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// test subject
# include "../StackFixedBlock.hpp"

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>
# include <vector>
# include <list>

// Project headers
# include "../../Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../Nginx_MPool_FixedElem/Nginx_MPool_FixedElem.hpp"
# include "../../../_Tests/test.h"

int TestPart1(int testNumber)
{
    // instantiation
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";
        StackFixedBlock<4096> pool;
        
        //Nginx_MemoryPool pool2;   // fails as expected, private default constructor
        //Nginx_MemoryPool copy(pool); // fails as expected, private copy constructor

        StackFixedBlock<4096> pool2;
        // pool2 = pool1; // fails as expected, private assignment operator
         
        pool.reset();       //all good

        void* ptr = pool.allocate(100);
        (void)ptr;
        
        // RAII, no leaks, everything is destructed

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
    
    
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";
        StackFixedBlock<4096> pool;
        
        Nginx_PoolAllocator<int, StackFixedBlock<4096> > alloc(pool);
        std::vector<int, Nginx_PoolAllocator<int, StackFixedBlock<4096> > > vec(alloc);
        
        vec.reserve(100);

        if (pool.getFreeSpace() != 4096 - 100 * sizeof(int))
            throw std::runtime_error("free space is not correct, got: " 
            + to_string(pool.getFreeSpace()) + " expected: " 
            + to_string(4096 - 100 * sizeof(int)) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


    // I FINALLY FOOLED A STD::LIST TO ALLOCATE MEMORY ON THE STACK
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";
        StackFixedBlock<4096> pool;
        
        Nginx_PoolAllocator<std::string, StackFixedBlock<4096> > alloc(pool);
        std::list<std::string, Nginx_PoolAllocator<std::string, StackFixedBlock<4096> > > list(alloc);
        

        if (pool.getFreeSpace() != 4096)
            throw std::runtime_error("free space is not correct, got: " 
            + to_string(pool.getFreeSpace()) + " expected: " 
            + to_string(4096 - 100 * sizeof(int)) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        list.push_back("big string that mallocs just to be sure that RAII is working");
        list.push_back("fits buf");

        if (pool.getFreeSpace() != 4096 - 2 * (sizeof(std::string) + 8 + 8))
            throw std::runtime_error("free space is not correct, got: " 
            + to_string(pool.getFreeSpace()) + " expected: " 
            + to_string(4096 - 2 * (sizeof(int) + 4 + 8 + 8)) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


    // I FINALLY FOOLED A STD::LIST TO ALLOCATE MEMORY ON THE STACK
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        const int elementCount = 100;
        const int poolSize = (sizeof(std::string) + 8 + 8) * elementCount;

        StackFixedBlock<poolSize> pool;
        
        Nginx_MPool_FixedElem<std::string, StackFixedBlock<poolSize> > alloc(pool, 100);

        std::list<std::string, Nginx_MPool_FixedElem<std::string, StackFixedBlock<poolSize> > > list(alloc);
        

        if (pool.getFreeSpace() != poolSize)
            throw std::runtime_error("free space is not correct, got: " 
            + to_string(pool.getFreeSpace()) + " expected: " 
            + to_string(4096 - 100 * sizeof(int)) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


        for (int i = 0; i < elementCount; i++)
            list.push_back("big string that mallocs just to be sure that RAII is working");

        if (pool.getFreeSpace() != 0)
            throw std::runtime_error("free space is not correct, got: " 
            + to_string(pool.getFreeSpace()) + " expected: " 
            + to_string(4096 - 2 * (sizeof(int) + 4 + 8 + 8)) + '\n'
            + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}