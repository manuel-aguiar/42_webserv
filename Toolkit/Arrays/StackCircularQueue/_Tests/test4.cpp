

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <list>

// Project headers
# include "../StackCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/TestHelpers.h"


int TestPart4(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		const int queueSize = 200;
		std::list<int> 					list;
		StackCircularQueue<int, queueSize> 			queue;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			queue.push_back(i);

			list.push_front(i);
			queue.push_front(i);
		}
		if (list.size() != queue.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(list.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackCircularQueue<int, queueSize>::iterator it = queue.begin();
		std::list<int>::iterator iter = list.begin();
		for ( ; it != queue.end() && iter != list.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
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

		const int queueSize = 100;
		std::list<ToolkitDummy> 							std;
		StackCircularQueue<ToolkitDummy, queueSize> 		queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.push_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackCircularQueue<ToolkitDummy, queueSize>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
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

		const int queueSize = 100;
		std::list<ToolkitDummy> 		std;
		StackCircularQueue<ToolkitDummy, queueSize> 		queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(std.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackCircularQueue<ToolkitDummy, queueSize>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
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

		const int queueSize = 200;
		std::list<ToolkitDummy> 								list;
		StackCircularQueue<ToolkitDummy, queueSize> 			queue;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			queue.emplace_back(i);
		}
		if (list.size() != queue.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(list.size()) + '\n'
            + TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


		StackCircularQueue<ToolkitDummy, queueSize>::iterator it = queue.begin();
		std::list<ToolkitDummy>::iterator iter = list.begin();

		for ( ; it != queue.end() && iter != list.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
				
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