

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <list>

// Project headers
# include "../HeapCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../TestHelpers/TestHelpers.h"


int TestPart4(int testNumber)
{
    try
	{
		TEST_INTRO(testNumber++);
		std::list<int> 					list;
		HeapCircularQueue<int> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			queue.push_back(i);

			list.push_front(i);
			queue.push_front(i);
		}

		EXPECT_EQUAL(list.size(), queue.size(), "size mismatch");

		HeapCircularQueue<int>::iterator it = queue.begin();
		std::list<int>::iterator iter = list.begin();
		for ( ; it != queue.end() && iter != list.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
    testNumber++;
	/******************* ***** ************************/

	try
	{
		TEST_INTRO(testNumber++);
		std::list<ToolkitDummy> 		std;
		HeapCircularQueue<ToolkitDummy> 		queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.push_back(i);
		}
		EXPECT_EQUAL(std.size(), queue.size(), "size mismatch");

		HeapCircularQueue<ToolkitDummy>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
    testNumber++;
	/******************* *****************************/

	try
	{
		TEST_INTRO(testNumber++);
		std::list<ToolkitDummy> 		std;
		HeapCircularQueue<ToolkitDummy> 		queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		EXPECT_EQUAL(std.size(), queue.size(), "size mismatch");

		HeapCircularQueue<ToolkitDummy>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
    testNumber++;



/******************* ***** ************************/

	try
	{
		TEST_INTRO(testNumber++);
		std::list<ToolkitDummy> 				list;
		HeapCircularQueue<ToolkitDummy> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			queue.emplace_back(i);
		}
		EXPECT_EQUAL(list.size(), queue.size(), "size mismatch");

		HeapCircularQueue<ToolkitDummy>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = list.begin();

		for ( ; it != queue.end() && iter != list.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}
    testNumber++;

/***************************************** */


    return (testNumber);
}