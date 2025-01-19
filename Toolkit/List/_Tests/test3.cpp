

// Project headers
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"
# include "../../_Tests/test.h"

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


		std::list<int> 	    std;
        List<int>           list;   

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list   <int>::iterator    iter = std.begin();
		List        <int>::iterator    it = list.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
	}

/******************* *** ************************/

	try
	{
		std::cout << "TEST " << testNumber++ << ": ";


		std::list<int> 	    std;
		List<int> 			list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list<int>::iterator    iter = std.begin();
		List<int>::iterator         it = list.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
		TEST_FAIL_INFO();
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

        if (stdList.size() != list.size())
            throw std::logic_error("size mismatch");

        std::list<int>::iterator stdIt = stdList.begin();
        List<int>::iterator listIt = list.begin();
        for ( ; stdIt != stdList.end() && listIt != list.end(); ++stdIt, ++listIt)
        {
            if (*stdIt != *listIt)
                throw std::logic_error("value mismatch");
        }

        if (stdList.size() == 99 && list.size() != 99)
            throw std::logic_error("Erase operation did not behave as expected");
            
        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "  FAILED: " << e.what() << std::endl;
        TEST_FAIL_INFO();
    }

    return (testNumber);
}