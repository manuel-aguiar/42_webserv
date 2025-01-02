/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:40:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 19:25:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <list>

// Project headers
# include "../FixedSizeQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"

# include "../../../_Tests/test.h"

//extern int TestPart1(int testNumber);
//extern int TestPart2(int testNumber);
//extern int TestPart3(int testNumber);
//extern int TestPart4(int testNumber);
//extern int TestPart5(int testNumber);

int ThisTest(int testNumber)
{

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		FixedSizeQueue<int> 			queue(2);
        queue.push_back(5);
		queue.push_front(2);

        if (queue[0] != 2)
            throw std::logic_error("front should be 2");
		if (queue[1] != 5)
			throw std::logic_error("back should be 5");

		for (FixedSizeQueue<int>::iterator it = queue.begin(); it != queue.end(); ++it)
		{
			std::cout << *it << std::endl;
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}

/******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<int> 		std;
		FixedSizeQueue<int> 			queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
            queue.push_back(i);
		}

		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

        FixedSizeQueue<int>::iterator itQueue = queue.begin();
        std::list<int>::iterator itList = std.begin();
        for ( ; itQueue != queue.end() && itList != std.end(); ++itQueue, ++itList)
        {
            if (*itQueue != *itList)
                throw std::logic_error("value mismatch");
        }

        FixedSizeQueue<int> assign(100);
        assign = queue;

		FixedSizeQueue<int>::iterator it = queue.begin();
		FixedSizeQueue<int>::iterator itAssign = assign.begin();
		std::cout << "testing copy assignment" << std::endl;
        
        for ( ; it != queue.end() && itAssign != assign.end(); ++it, ++itAssign)
        {
            if (*it != *itAssign)
                throw std::logic_error("copy assignment, value mismatch");
        }

        if (queue.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;

/******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<int> 		std;
		FixedSizeQueue<int> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
            std.push_back(i);
            std.pop_front();
            std.push_front(i);
            std.pop_back();

			queue.push_back(i);
            queue.push_back(i);
            queue.pop_front();
            queue.push_front(i);
            queue.pop_back();
		}

		for (int i = 0; i < 50; ++i)
		{
            std.pop_front();
            queue.pop_front();
		}

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
            std.push_back(i);
            std.pop_front();
            std.push_front(i);
            std.pop_back();

			queue.push_back(i);
            queue.push_back(i);
            queue.pop_front();
            queue.push_front(i);
            queue.pop_back();
		}

		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

        FixedSizeQueue<int>::iterator itQueue = queue.begin();
        std::list<int>::iterator itList = std.begin();
        for ( ; itQueue != queue.end() && itList != std.end(); ++itQueue, ++itList)
        {
            if (*itQueue != *itList)
                throw std::logic_error("value mismatch");
        }

        FixedSizeQueue<int> assign(200);
        assign = queue;

		FixedSizeQueue<int>::iterator it = queue.begin();
		FixedSizeQueue<int>::iterator itAssign = assign.begin();
		std::cout << "testing copy assignment" << std::endl;
        
        for ( ; it != queue.end() && itAssign != assign.end(); ++it, ++itAssign)
        {
            if (*it != *itAssign)
                throw std::logic_error("copy assignment, value mismatch");
        }

        if (queue.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;

    return (testNumber);
}

int main()
{
    int testNumber = 1;
    
	std::cout << "\n*************** FixedSizeQueue tests ***************" << std::endl;
	
    testNumber = ThisTest(testNumber);
    //testNumber = TestPart3(testNumber);
    //testNumber = TestPart4(testNumber);
    //testNumber = TestPart5(testNumber);

	std::cout << "*********************************************\n" << std::endl;

    return (0);
}

/*
    c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && ./main
*/
