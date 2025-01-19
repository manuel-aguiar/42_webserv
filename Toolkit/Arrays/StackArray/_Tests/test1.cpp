/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:00:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/19 12:13:11 by mmaria-d         ###   ########.fr       */
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


int TestPart1(int testNumber)
{
	/*********************** ************************/
	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		const int arraySize = 100;
		StackArray<int, arraySize> array;

		if (array.capacity() != arraySize)
			throw std::runtime_error("size mismatch, got: " + to_string(array.capacity()) + " expected: " + to_string(arraySize) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		for (int i = 0; i < 100; ++i)
		{
			array.emplace_back(i);
		}

		if (array.size() != arraySize)
			throw std::runtime_error("size mismatch, got: " + to_string(array.size()) + " expected: " + to_string(arraySize) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		for (int i = 0; i < 100; ++i)
		{
			if (array[i] != i)
				throw std::runtime_error("value mismatch, got: " + to_string(array[i]) + " expected: " + to_string(i) + '\n'
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/*********************** ************************/
	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		const int arraySize = 100;
		const int copyInit = 123;

		// copy constructs the elements based on the one element passed
		StackArray<int, arraySize> array(copyInit);
		if (array.size() != arraySize)

			throw std::runtime_error("size mismatch, got: " + to_string(array.size()) + " expected: " + to_string(arraySize) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		for (size_t i = 0; i < array.size(); ++i)
		{
			if (array[i] != copyInit)
				throw std::runtime_error("value mismatch, got: " + to_string(array[i]) + " expected: " + to_string(copyInit) + '\n'
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


	/*************************************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		const int arraySize = 100;

		StackArray<ToolkitDummy, arraySize> array;
		for (size_t i = 0; i < array.capacity(); ++i)
		{
			array.emplace_back(i);
		}

		if (array.size() != arraySize)
			throw std::runtime_error("size mismatch, got: " + to_string(array.size()) + " expected: " + to_string(arraySize) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
			
		for (size_t i = 0; i < array.size(); ++i)
		{
			if (array[i] != i)
				throw std::runtime_error("value mismatch, got: " + to_string(array[i]) + " expected: " + to_string(i) + '\n'
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/******************* ***** ************************/

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
			throw std::runtime_error("size mismatch, got: " + to_string(array.size()) + " expected: " + to_string(99) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (array.back() != 98)
			throw std::runtime_error("value mismatch, got: " + to_string(array.back()) + " expected: " + to_string(98) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
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
			throw std::runtime_error("incorrect front element, got: " + to_string(array.front()) + " expected: " + to_string(0) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		if (array.back() != 99)
			throw std::runtime_error("incorrect back element, got: " + to_string(array.back()) + " expected: " + to_string(99) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
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
			throw std::runtime_error("size mismatch, got: " + to_string(array.size()) + " expected: " + to_string(3) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		array.emplace_back(4);
		array.emplace_back(5);
		if (array.size() != 5)
			throw std::runtime_error("size mismatch, got: " + to_string(array.size()) + " expected: " + to_string(5) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
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
			throw std::runtime_error("incorrect value at iterator begin, got: " + to_string(*it) + " expected: " + to_string(1) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		it++;
		if (*it != 2)
		throw std::runtime_error("incorrect value at second iterator, got: " + to_string(*it) + " expected: " + to_string(2) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		it++;
		if (*it != 3)
			throw std::runtime_error("incorrect value at third iterator, got: " + to_string(*it) + " expected: " + to_string(3) + '\n'
			+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		it++;

		if (it != array.end())
			throw std::runtime_error("iterator should be at end but is not");

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/
	/* just strings... */
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		StackArray<std::string, 3> array;
		array.emplace_back("Hello");
		array.emplace_back("World");
		array.emplace_back("StackArray");

		if (array[0] != "Hello" || array[1] != "World" || array[2] != "StackArray")
			throw std::logic_error("value mismatch\n" + FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}
