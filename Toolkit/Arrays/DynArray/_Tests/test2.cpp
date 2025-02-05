// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// Project headers
# include "../DynArray.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../TestHelpers/TestHelpers.h"

class EmplaceTwo
{
    public:
        EmplaceTwo(const std::string& name, const int number) : m_name(name), m_number(number) {};
		~EmplaceTwo() {};
		
        bool operator==(const EmplaceTwo& other)
        {
            return (m_name == other.m_name && m_number == other.m_number);
        }

        bool operator!=(const EmplaceTwo& other)
        {
            return (!(*this == other));
        }
        
        std::string m_name;
        int         m_number;
};


int TestPart2(int testNumber)
{
    try
	{
		TEST_INTRO(testNumber++);
		std::vector<int> std;
		DynArray<int> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

		DynArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it == *iter, true, "value mismatch");
		}

        DynArray<int> assign;
        assign = array;

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy assignment, value mismatch");
        }

        EXPECT_EQUAL(std.size(), assign.size(), "copy assignment, size mismatch");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    try
	{
		TEST_INTRO(testNumber++);
		std::vector<int> std;
		DynArray<int> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

		DynArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it == *iter, true, "value mismatch");
		}

        DynArray<int> assign(array);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy constructor, value mismatch");
        }

        EXPECT_EQUAL(std.size(), assign.size(), "copy constructor, size mismatch");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    try
	{
		TEST_INTRO(testNumber++);
		std::vector<int> std;
		DynArray<int> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

		DynArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			EXPECT_EQUAL(*it == *iter, true, "value mismatch");
		}

        DynArray<int> assign;

        assign.push_back(1);
        assign.push_back(2);

        assign.move(array);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "::move failed, value mismatch");
        }

        EXPECT_EQUAL(std.size(), assign.size(), "::move failed, size mismatch");

        EXPECT_EQUAL(array.size(), 0, "::move failed, source array not empty");

        array.push_back(1);
        array.push_back(2);

        EXPECT_EQUAL(std.size(), assign.size(), "::move failed, the movedFrom array influenced the moveTo");

        array.clear();

        EXPECT_EQUAL(array.size(), 0, "::clear failed, array not empty");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

/******************************************************************** */
/* Emplace two */

    try
	{
		TEST_INTRO(testNumber++);
		std::vector<EmplaceTwo> std;
		DynArray<EmplaceTwo> array;
		
		for (int i = 0; i < 2; ++i)
		{
			std.push_back(EmplaceTwo("name", i));
			array.push_back(EmplaceTwo("name", i));
		}

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    return (testNumber);
}
