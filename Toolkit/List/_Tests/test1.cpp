// Project headers
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"
#include "../../TestHelpers/TestHelpers.h"

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
		TEST_INTRO(testNumber++);
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 			list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.push_back(i);
		}
		EXPECT_EQUAL(std.size(), list.size(), "size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/************************ ************************/

	try
	{
		TEST_INTRO(testNumber++);
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 	list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.push_front(i);
		}
		EXPECT_EQUAL(std.size(), list.size(), "size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* *** ************************/

	try
	{
		TEST_INTRO(testNumber++);
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 	list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.emplace_front(i);
		}
		EXPECT_EQUAL(std.size(), list.size(), "size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	try
	{
		TEST_INTRO(testNumber++);
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 	list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		EXPECT_EQUAL(std.size(), list.size(), "size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* *** ************************/

	try
	{
		TEST_INTRO(testNumber++);
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 	list;

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
		EXPECT_EQUAL(std.size(), list.size(), "size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* *** ************************/

	try
	{
		TEST_INTRO(testNumber++);
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
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* *** ************************/

	try
	{
		TEST_INTRO(testNumber++);
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

		EXPECT_EQUAL(list.size(), copy.size(), "size mismatch");

		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* *** ************************/

	try
	{
		TEST_INTRO(testNumber++);

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
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	/******************* *** ************************/

	try
	{
		TEST_INTRO(testNumber++);
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

		EXPECT_EQUAL(list.size(), copy.size(), "size mismatch");

		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

	return (testNumber);
}