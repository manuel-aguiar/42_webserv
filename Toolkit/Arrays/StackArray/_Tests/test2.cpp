/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 08:45:18 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 09:10:01 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>

// Project headers
#include "../StackArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/test.h"

int TestPart2(int testNumber)
{
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<int> 		std;
		StackArray<int, 100> 	array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		StackArray<int, 100>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}

        StackArray<int, 100> assign(10); // default construct 10 out of the 100 cap
        assign = array;

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy assignment, value mismatch");
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch");


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
		std::vector<int> 		std;
		StackArray<int, 100> 	array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		StackArray<int, 100>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}

        StackArray<int, 100> copy(array);

        it = copy.begin();
        iter = std.begin();
        
        for ( ; it != copy.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy constructor, value mismatch");
        }

        if (std.size() != copy.size())
			throw std::logic_error("copy constructor, size mismatch");


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
		std::vector<int> 		std;
		StackArray<int, 100> 	array(100, 5);		// default construct 100 elements with value 5

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(5);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		StackArray<int, 100>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}

        StackArray<int, 100> copy(array);

        it = copy.begin();
        iter = std.begin();
        
        for ( ; it != copy.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy constructor, value mismatch");
        }

        if (std.size() != copy.size())
			throw std::logic_error("copy constructor, size mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}

	return (testNumber);
}