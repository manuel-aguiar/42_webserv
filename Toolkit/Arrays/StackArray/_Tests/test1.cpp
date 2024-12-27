/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:00:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 08:44:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

// Project headers
#include "../StackArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/test.h"


template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
int TestPart1(int testNumber)
{
	/******************* TEST 1 ************************/
	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		
		StackArray<int, 100> array;
		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}
		if (array.size() != 100)
			throw std::logic_error("size mismatch");
		for (int i = 0; i < 100; ++i)
		{
			if (array[i] != i)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

	/******************* TEST 2 ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		StackArray<ToolkitDummy, 100> array;
		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}
		if (array.size() != 100)
			throw std::logic_error("size mismatch");
		for (size_t i = 0; i < 100; ++i)
		{
			if (array[i] != i)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

	/******************* TEST 3 ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		StackArray<int, 100> array;
		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}
		array.pop_back();
		if (array.size() != 99)
			throw std::logic_error("size mismatch after pop_back");

		if (array.back() != 98)
			throw std::logic_error("wrong element at back after pop_back");

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
		StackArray<int, 100> array;
		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}

		// Check front and back
		if (array.front() != 0)
			throw std::logic_error("incorrect front element");
		if (array.back() != 99)
			throw std::logic_error("incorrect back element");

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
		StackArray<int, 5> array;
		array.emplace_back(1);
		array.emplace_back(2);
		array.emplace_back(3);
		if (array.size() != 3)
			throw std::logic_error("size mismatch");

		array.emplace_back(4);
		array.emplace_back(5);
		if (array.size() != 5)
			throw std::logic_error("size mismatch after adding 5 elements");

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
		StackArray<int, 5> array;
		array.emplace_back(1);
		array.emplace_back(2);
		array.emplace_back(3);

		// Test iterator
		StackArray<int, 5>::iterator it = array.begin();
		if (*it != 1)
			throw std::logic_error("incorrect value at iterator begin");

		it++;
		if (*it != 2)
			throw std::logic_error("incorrect value at iterator second element");

		it++;
		if (*it != 3)
			throw std::logic_error("incorrect value at iterator third element");

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
		StackArray<std::string, 3> array;
		array.emplace_back("Hello");
		array.emplace_back("World");
		array.emplace_back("StackArray");

		if (array[0] != "Hello" || array[1] != "World" || array[2] != "StackArray")
			throw std::logic_error("value mismatch");

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

    return (testNumber);
}
