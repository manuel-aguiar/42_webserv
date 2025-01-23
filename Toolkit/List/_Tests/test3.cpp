// Project headers
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/TestHelpers.h"

// C++ headers
#include <list>
#include <iostream>
#include <cstring>
#include <stdexcept>

int TestPart3(int testNumber)
{
    /******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		std::list<int>  std;
        List<int>     list;   

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		TestHelpers::assertEqual(std.size(), list.size(), "size mismatch", __FILE__, __LINE__, __func__);

		std::list<int>::iterator iter = std.begin();
		List<int>::iterator it = list.begin();
		for (; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __func__);
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

		std::list<int> std;
		List<int> list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		TestHelpers::assertEqual(std.size(), list.size(), "size mismatch", __FILE__, __LINE__, __func__);

		std::list<int>::iterator iter = std.begin();
		List<int>::iterator it = list.begin();
		for (; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __func__);
		}
		std::cout << "\tPASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "\tFAILED: " << e.what()  << std::endl;
	}

/******************* TEST ::erase() ************************/

    try
    {
        std::cout << "TEST " << testNumber++ << ": ";

        std::list<int> stdList;
        List<int> list;

        for (int i = 0; i < 100; ++i)
        {
            stdList.push_back(i);
            list.emplace_back(i);
        }

        std::list<int>::iterator stdIter = stdList.begin();
        std::advance(stdIter, 10);
        stdList.erase(stdIter);

        List<int>::iterator listIter = list.begin();
        std::advance(listIter, 10);
        list.erase(listIter);

        TestHelpers::assertEqual(stdList.size(), list.size(), "size mismatch", __FILE__, __LINE__, __func__);

        std::list<int>::iterator stdIt = stdList.begin();
        List<int>::iterator listIt = list.begin();
        for (; stdIt != stdList.end() && listIt != list.end(); ++stdIt, ++listIt)
        {
            TestHelpers::assertEqual(*stdIt, *listIt, "value mismatch", __FILE__, __LINE__, __func__);
        }

        TestHelpers::assertEqual(list.size(), (size_t)99, "Erase operation did not behave as expected", __FILE__, __LINE__, __func__);
            
        std::cout << "\tPASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "  FAILED: " << e.what() << std::endl;
    }

    return (testNumber);
}
