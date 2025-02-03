


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
# include "../../../TestHelpers/TestHelpers.h"


int TestPart3(int testNumber)
{
    try
	{
		TEST_INTRO(testNumber++);

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

		EXPECT_EQUAL(std.size(), queue.size(), "size mismatch");

		StackCircularQueue<ToolkitBase*, queueSize>::iterator it = queue.begin();
		std::list<ToolkitBase*>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL((*it)->getValue() == (*iter)->getValue(), true, "value mismatch");
		}

		it = queue.begin();
		iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			delete (*it);
			delete (*iter);
		}


		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    return (testNumber);
}