

// Project headers
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/TestHelpers.h"

// C++ headers
#include <list>
#include <iostream>
#include <cstring>
#include <stdexcept>


int StressTest(int testNumber)
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
			if (*it != *iter)
				throw std::logic_error("copy constructor, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy constructor, same pointer");
		}

		if (list.size() != copy.size())
			throw std::logic_error("copy constructor, size mismatch");

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
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
			if (*it != *iter)
				throw std::logic_error("copy assignment, value mismatch");
			if (it.getPtr() == iter.getPtr())
				throw std::logic_error("copy constructor, same pointer");
		}

		if (list.size() != copy.size())
			throw std::logic_error("copy assignment, size mismatch");

		std::cout << "	PASSED" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

	return (testNumber);
}