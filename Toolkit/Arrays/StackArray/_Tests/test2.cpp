

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
# include "../../../_Tests/TestHelpers.h"

int StressTest(int testNumber)
{
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		const int arraySize = 100;

		std::vector<int> 			std;
		StackArray<int, arraySize> 	array;

		for (int i = 0; i < arraySize; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::runtime_error("size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackArray<int, 100>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();

		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::runtime_error("value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        StackArray<int, 100> assign;

		assign.push_back(-123);
		assign.push_back(-456);

        assign = array;

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
			if (*it != *iter)
				throw std::runtime_error("copy assignment value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != assign.size())
			throw std::runtime_error("copy assignment size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(assign.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	/********************************************* */

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		const int arraySize = 100;

		std::vector<int> 		std;
		StackArray<int, arraySize> 	array;

		for (size_t i = 0; i < array.capacity(); ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::runtime_error("size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackArray<int, 100>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::runtime_error("value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        StackArray<int, 100> copy(array);

        it = copy.begin();
        iter = std.begin();
        
        for ( ; it != copy.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::runtime_error("copy constructor value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != copy.size())
			throw std::runtime_error("copy constructor size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(copy.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


	/********************************************* */

	
	try
	{
		const int arraySize = 100;

		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<int> 			std;
		StackArray<int, arraySize> 	array(5);		// all elements are copy constructed from "5"s

		for (size_t i = 0; i < array.capacity(); ++i)
		{
			std.push_back(5);
		}

		if (std.size() != array.size())
			throw std::runtime_error("size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		StackArray<int, arraySize>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::runtime_error("value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        StackArray<int, arraySize> copy(array);

        it = copy.begin();
        iter = std.begin();
        
        for ( ; it != copy.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::runtime_error("copy constructor value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != copy.size())
			throw std::runtime_error("copy constructor size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(copy.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}