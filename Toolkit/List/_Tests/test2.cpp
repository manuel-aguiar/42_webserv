// Project headers
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/TestHelpers.h"

// C++ headers
#include <list>
#include <iostream>
#include <cstring>
#include <stdexcept>


int TestPart2(int testNumber)
{
	/*********************** ************************/
	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		List<int> list;

		for (int i = 0; i < 100; ++i)
			list.emplace_back(i);

		List<int> copy(list);
		List<int>::iterator iter = copy.begin();
		List<int>::iterator it = list.begin();

		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "copy constructor, value mismatch", __FILE__, __LINE__, __func__);
			TestHelpers::assertEqual(it.getPtr() != iter.getPtr(), true, "copy constructor, same pointer", __FILE__, __LINE__, __func__);
		}

		TestHelpers::assertEqual(list.size(), copy.size(), "copy constructor, size mismatch", __FILE__, __LINE__, __func__);

		std::cout << "\tPASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}
	

	std::cout << "TEST " << testNumber++ << ": ";
	try
	{
		List<int> list;

		for (int i = 0; i < 100; ++i)
			list.emplace_back(i);

		List<int> copy;

		copy = list;
		List<int>::iterator iter = copy.begin();
		List<int>::iterator it = list.begin();

		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "copy assignment, value mismatch", __FILE__, __LINE__, __func__);
			TestHelpers::assertEqual(it.getPtr() != iter.getPtr(), true, "copy constructor, same pointer", __FILE__, __LINE__, __func__);
		}

		TestHelpers::assertEqual(list.size(), copy.size(), "copy assignment, size mismatch", __FILE__, __LINE__, __func__);

		std::cout << "\tPASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}
