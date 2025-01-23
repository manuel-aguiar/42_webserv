


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


int TestPart3(int testNumber)
{
    try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitBase*> 				std;
		HeapCircularQueue<ToolkitBase*> 		queue(300);

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


		TestHelpers::assertEqual(std.size(), queue.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		HeapCircularQueue<ToolkitBase*>::iterator it = queue.begin();
		std::list<ToolkitBase*>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual((*it)->getValue() == (*iter)->getValue(), true, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
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