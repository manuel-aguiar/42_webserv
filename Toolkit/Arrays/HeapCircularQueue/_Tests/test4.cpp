/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test4.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:06:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 10:52:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <list>

// Project headers
# include "../HeapCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/test.h"


int TestPart4(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<int> 				list;
		HeapCircularQueue<int> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			queue.push_back(i);

			list.push_front(i);
			queue.push_front(i);
		}
		if (list.size() != queue.size())
			throw std::logic_error("size mismatch");

		HeapCircularQueue<int>::iterator it = queue.begin();
		std::list<int>::iterator iter = list.begin();
		for ( ; it != queue.end() && iter != list.end(); ++it, ++iter)
		{
			std::cout << "list: " << *iter << " queue: " << *it << std::endl;
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;
	/******************* ***** ************************/

	try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<ToolkitDummy> 		std;
		HeapCircularQueue<ToolkitDummy> 		queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.push_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

		HeapCircularQueue<ToolkitDummy>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;
	/******************* *****************************/

	try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<ToolkitDummy> 		std;
		HeapCircularQueue<ToolkitDummy> 		queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

		HeapCircularQueue<ToolkitDummy>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;



/******************* ***** ************************/

	try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<ToolkitDummy> 				list;
		HeapCircularQueue<ToolkitDummy> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			queue.emplace_back(i);
		}
		if (list.size() != queue.size())
			throw std::logic_error("size mismatch");


		HeapCircularQueue<ToolkitDummy>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = list.begin();

		for ( ; it != queue.end() && iter != list.end(); ++it, ++iter)
		{
			std::cout << "list: " << *iter <<"queue: " << *it << std::endl;
			if (*it != *iter)
			{
				std::cout << "list: " << *iter << "queue: " << *it << std::endl;
				throw std::logic_error("value mismatch");
			}
				
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;

/***************************************** */


    return (testNumber);
}