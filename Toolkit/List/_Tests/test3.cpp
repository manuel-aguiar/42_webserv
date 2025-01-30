// Project headers
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"
# include "../../TestHelpers/TestHelpers.h"

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
		TEST_INTRO(testNumber++);

		std::list<int>  std;
        List<int>     list;   

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		EXPECT_EQUAL(std.size(), list.size(), "size mismatch");

		std::list<int>::iterator iter = std.begin();
		List<int>::iterator it = list.begin();
		for (; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

/******************* *** ************************/

	try
	{
		TEST_INTRO(testNumber++);

		std::list<int> std;
		List<int> list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		EXPECT_EQUAL(std.size(), list.size(), "size mismatch");

		std::list<int>::iterator iter = std.begin();
		List<int>::iterator it = list.begin();
		for (; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it, *iter, "value mismatch");
		}
		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

/******************* TEST ::erase() ************************/

    try
    {
        TEST_INTRO(testNumber++);

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

        EXPECT_EQUAL(stdList.size(), list.size(), "size mismatch");

        std::list<int>::iterator stdIt = stdList.begin();
        List<int>::iterator listIt = list.begin();
        for (; stdIt != stdList.end() && listIt != list.end(); ++stdIt, ++listIt)
        {
            EXPECT_EQUAL(*stdIt, *listIt, "value mismatch");
        }

        EXPECT_EQUAL(list.size(), 99, "Erase operation did not behave as expected");
            
        TEST_PASSED;
    }
    catch (const std::exception& e)
    {
        std::cout << "  FAILED: " << e.what() << std::endl;
    }

    return (testNumber);
}
