/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:12:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/13 00:09:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
# include <exception>
# include <stdexcept>
# include <iostream>
# include <vector>

// Project headers
# include "../../Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../Heap_MemoryPool/Heap_MemoryPool.hpp"
# include "../../Nginx_PoolAllocator/Nginx_PoolAllocator.hpp"
# include "../../../_Tests/test.h"

int TestPart1(int testNumber)
{
    // instantiation
    try
    {
        std::cout << "TEST " << testNumber++ << ": ";
        Heap_MemoryPool pool(4096);
        
        //Nginx_MemoryPool pool2;   // fails as expected, private default constructor
        //Nginx_MemoryPool copy(pool); // fails as expected, private copy constructor

        Heap_MemoryPool pool2(4096);
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

        
        Heap_MemoryPool pool(4096);
        
        Nginx_PoolAllocator<int, Heap_MemoryPool > alloc(pool);
        std::vector<int, Nginx_PoolAllocator<int, Heap_MemoryPool > > vec(alloc);
        
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

    return (testNumber);
}