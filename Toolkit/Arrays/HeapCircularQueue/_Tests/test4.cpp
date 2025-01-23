

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
# include "../../../_Tests/TestHelpers.h"


int TestPart4(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<int> 					list;
		HeapCircularQueue<int> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			queue.push_back(i);

			list.push_front(i);
			queue.push_front(i);
		}

		TestHelpers::assertEqual(list.size(), queue.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		HeapCircularQueue<int>::iterator it = queue.begin();
		std::list<int>::iterator iter = list.begin();
		for ( ; it != queue.end() && iter != list.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
    testNumber++;
	/******************* ***** ************************/

	try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<ToolkitDummy> 		std;
		HeapCircularQueue<ToolkitDummy> 		queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.push_back(i);
		}
		TestHelpers::assertEqual(std.size(), queue.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		HeapCircularQueue<ToolkitDummy>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
    testNumber++;
	/******************* *****************************/

	try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<ToolkitDummy> 		std;
		HeapCircularQueue<ToolkitDummy> 		queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		TestHelpers::assertEqual(std.size(), queue.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		HeapCircularQueue<ToolkitDummy>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
    testNumber++;



/******************* ***** ************************/

	try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::list<ToolkitDummy> 				list;
		HeapCircularQueue<ToolkitDummy> 			queue(200);

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			queue.emplace_back(i);
		}
		TestHelpers::assertEqual(list.size(), queue.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		HeapCircularQueue<ToolkitDummy>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = list.begin();

		for ( ; it != queue.end() && iter != list.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}
    testNumber++;

/***************************************** */


    return (testNumber);
}