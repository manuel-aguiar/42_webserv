/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:06:14 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 14:55:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// Project headers
# include "../FixedSizeQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/test.h"


int TestPart1(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::vector<int> 				std;
		FixedSizeQueue<int> 			queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.push_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

		FixedSizeQueue<int>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();
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
		std::vector<ToolkitDummy> 		std;
		FixedSizeQueue<ToolkitDummy> 		queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.push_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

		FixedSizeQueue<ToolkitDummy>::iterator it = queue.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
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
		std::vector<ToolkitDummy> 		std;
		FixedSizeQueue<ToolkitDummy> 		queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

		FixedSizeQueue<ToolkitDummy>::iterator it = queue.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
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
		std::vector<ToolkitDummy> 			std;
		FixedSizeQueue<ToolkitDummy> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.push_back(std[0]);
			queue.emplace_back(i);
			queue.emplace_back(queue[0]);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

		FixedSizeQueue<ToolkitDummy>::iterator it = queue.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
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

/***************************************** */


    return (testNumber);
}