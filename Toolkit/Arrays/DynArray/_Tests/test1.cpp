// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// Project headers
# include "../DynArray.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../TestHelpers/TestHelpers.h"

int TestPart1(int testNumber)
{

	/*********************************************** */

    try
	{
		TEST_INTRO(testNumber++);
		std::vector<int> std;
		DynArray<int> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		EXPECT_EQUAL(array.size(), std.size(), "size mismatch");

		DynArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for (; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			bool resultValueMatch = (*it == *iter);
			EXPECT_EQUAL(resultValueMatch, true, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
	/***************************************/

	try
	{
		TEST_INTRO(testNumber++);
		std::vector<ToolkitDummy> std;
		DynArray<ToolkitDummy> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}
		EXPECT_EQUAL(array.size(), std.size(), "size mismatch");

		DynArray<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
		for (; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			bool resultValueMatch = (*it == *iter);
			EXPECT_EQUAL(resultValueMatch, true, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
	/*******************  ************************/

	try
	{
		TEST_INTRO(testNumber++);
		std::vector<ToolkitDummy> std;
		DynArray<ToolkitDummy> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		EXPECT_EQUAL(array.size(), std.size(), "size mismatch");

		DynArray<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
		for (; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			bool resultValueMatch = (*it == *iter);
			EXPECT_EQUAL(resultValueMatch, true, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}


	/******************* ************************/

	try
	{
		TEST_INTRO(testNumber++);
		std::vector<ToolkitDummy> std;
		DynArray<ToolkitDummy> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.push_back(std[0]);
			array.emplace_back(i);
			array.emplace_back(array[0]);
		}
		EXPECT_EQUAL(array.size(), std.size(), "size mismatch");

		DynArray<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
		for (; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			bool resultValueMatch = (*it == *iter);
			EXPECT_EQUAL(resultValueMatch, true, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    return (testNumber);
}
