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
# include "../../../TestHelpers/TestHelpers.h"

int TestPart1(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<int> 		std;
		HeapArray<int> 			array(100);

		/************* Insert elements and test size *************/
		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}

		TestHelpers::assertEqual(std.size(), array.size(), "size mismatch after pushing elements", __FILE__, __LINE__, __FUNCTION__);

		/************* Compare elements *************/
		HeapArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch during iteration", __FILE__, __LINE__, __FUNCTION__);
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

		/************* Insert elements and test size *************/
		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}

		TestHelpers::assertEqual(std.size(), array.size(), "size mismatch after pushing ToolkitDummy elements", __FILE__, __LINE__, __FUNCTION__);

		/************* Compare elements *************/
		HeapArray<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch during iteration with ToolkitDummy", __FILE__, __LINE__, __FUNCTION__);
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

		/************* Insert elements with emplace_back and test size *************/
		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}

		TestHelpers::assertEqual(std.size(), array.size(), "size mismatch after emplacing ToolkitDummy elements", __FILE__, __LINE__, __FUNCTION__);

		/************* Compare elements *************/
		HeapArray<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch during emplace_back iteration with ToolkitDummy", __FILE__, __LINE__, __FUNCTION__);
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

		/************* Insert elements and duplicate some *************/
		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.push_back(std[0]);
			array.emplace_back(i);
			array.emplace_back(array[0]);
		}

		TestHelpers::assertEqual(std.size(), array.size(), "size mismatch after inserting and duplicating ToolkitDummy elements", __FILE__, __LINE__, __FUNCTION__);

		/************* Compare elements *************/
		HeapArray<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch during iteration with duplicated ToolkitDummy", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}
