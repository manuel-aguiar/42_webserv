


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


int TestPart3(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		const int queueSize = 300;

		std::list<ToolkitBase*> 				std;
		StackCircularQueue<ToolkitBase*, queueSize> 		queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(new ToolkitBase(i));
			queue.push_back(new ToolkitBase(i));

			std.push_front(new ToolkitDerived(i));
			queue.push_front(new ToolkitDerived(i));

			std.push_back(new ToolkitDerived(i));
			queue.push_back(new ToolkitDerived(i));

			delete std.back();
			delete queue.back();

			std.pop_back();
			queue.pop_back();
			std.push_front(new ToolkitBase(i));
			queue.push_front(new ToolkitBase(i));
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(std.size())
			+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackCircularQueue<ToolkitBase*, queueSize>::iterator it = queue.begin();
		std::list<ToolkitBase*>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (**it != **iter)
				throw std::logic_error("value mismatch, got " + TestHelpers::to_string((*it)->getValue()) + " expected: " + TestHelpers::to_string((*iter)->getValue())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

		it = queue.begin();
		iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			delete (*it);
			delete (*iter);
		}


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}