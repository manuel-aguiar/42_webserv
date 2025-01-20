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
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 			list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.push_back(i);
		}
		TestHelpers::assertEqual(std.size(), list.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	/************************ ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 	list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.push_front(i);
		}
		TestHelpers::assertEqual(std.size(), list.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 	list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.emplace_front(i);
		}
		TestHelpers::assertEqual(std.size(), list.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 	list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		TestHelpers::assertEqual(std.size(), list.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
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
		TestHelpers::assertEqual(std.size(), list.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
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
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
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

		TestHelpers::assertEqual(list.size(), copy.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

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
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
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

		TestHelpers::assertEqual(list.size(), copy.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}