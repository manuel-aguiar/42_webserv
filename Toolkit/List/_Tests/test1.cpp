/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:57:10 by manuel            #+#    #+#             */
/*   Updated: 2025/01/02 11:29:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/test.h"

// C++ headers
#include <list>
#include <iostream>
#include <cstring>
#include <stdexcept>


int TestPart1(int testNumber)
{
	/*********************** ************************/
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 			list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.push_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	/************************ ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.push_front(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.emplace_front(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.push_back(i);

			std.push_back(i + 100);
			list.push_back(i + 100);

			std.pop_front();
			list.pop_front();

			std.push_front(i + 100);
			list.push_front(i + 100);

			std.pop_back();
			list.pop_back();

			std.push_back(i + 100);
			list.push_back(i + 100);

			std.push_back(i + 100);
			list.push_back(i + 100);

			std.pop_front();
			list.pop_front();


		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
		}

		List<ToolkitDummy> copy;

		copy = list;
		List<ToolkitDummy>::iterator iter = copy.begin();
		List<ToolkitDummy>::iterator it = list.begin();
		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
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

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		List<ToolkitDummy> 		copy;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			copy.push_back(i + 1);
		}

		copy.push_back(1000);
		copy.push_back(1000);
		copy.push_back(1000);
		copy.push_back(1000);

		copy = list;
		List<ToolkitDummy>::iterator iter = copy.begin();
		List<ToolkitDummy>::iterator it = list.begin();

		if (list.size() != copy.size())
		{
			std::cout << "list size: " << list.size() << " copy size: " << copy.size() << std::endl;

			throw std::logic_error("size mismatch");
		}


		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
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

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
		}

		List<ToolkitDummy> 		copy(list);
		List<ToolkitDummy>::iterator iter = copy.begin();
		List<ToolkitDummy>::iterator it = list.begin();
		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
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

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		List<ToolkitDummy> 		copy;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			copy.push_back(i + 1);
		}

		copy.pop_front();
		copy.pop_front();
		copy.pop_front();
		copy.pop_front();

		copy = list;
		List<ToolkitDummy>::iterator iter = copy.begin();
		List<ToolkitDummy>::iterator it = list.begin();

		if (list.size() != copy.size())
		{
			std::cout << "list size: " << list.size() << " copy size: " << copy.size() << std::endl;

			throw std::logic_error("size mismatch");
		}


		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
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

	return (testNumber);
}
