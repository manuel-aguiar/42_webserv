
// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// Project headers
# include "../HeapArray.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/TestHelpers.h"


int TestPart1(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<int> 		std;
		HeapArray<int> 			array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}
		if (std.size() != array.size())
			throw std::runtime_error("size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::runtime_error("value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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
		std::vector<ToolkitDummy> 		std;
		HeapArray<ToolkitDummy> 		array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}
		if (std.size() != array.size())
			throw std::runtime_error("size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapArray<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::runtime_error("value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
	/******************* *****************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<ToolkitDummy> 		std;
		HeapArray<ToolkitDummy> 		array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::runtime_error("size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapArray<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::runtime_error("value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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
		std::vector<ToolkitDummy> 			std;
		HeapArray<ToolkitDummy> 			array(200);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.push_back(std[0]);
			array.emplace_back(i);
			array.emplace_back(array[0]);
		}
		if (std.size() != array.size())
			throw std::runtime_error("size mismatch, got: " + TestHelpers::to_string(array.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapArray<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::runtime_error("value mismatch, got: " + TestHelpers::to_string(*it) + " expected: " + TestHelpers::to_string(*iter) + '\n'
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

/***************************************** */


    return (testNumber);
}