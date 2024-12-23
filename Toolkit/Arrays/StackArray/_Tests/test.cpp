/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:00:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 11:38:19 by mmaria-d         ###   ########.fr       */
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

template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
int main()
{
	/******************* TEST 1 ************************/
	std::cout << "\n*************** StackArray tests ***************" << std::endl;
	try
	{
		std::cout << "TEST 1: ";
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
	}

	/******************* TEST 2 ************************/

	try
	{
		std::cout << "TEST 2: ";
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
	}

	/******************* TEST 3 ************************/

	try
	{
		std::cout << "TEST 3: ";
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
	}

	/******************* TEST 4 ************************/

	try
	{
		std::cout << "TEST 4: ";
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
	}

	/******************* TEST 5 ************************/

	try
	{
		std::cout << "TEST 5: ";
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
	}


	/******************* TEST 6 ************************/

	try
	{
		std::cout << "TEST 6: ";
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
	}

	/******************* TEST 7 ************************/

	try
	{
		std::cout << "TEST 7: ";
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
	}

	std::cout << "*********************************************\n" << std::endl;

    return (0);
}
