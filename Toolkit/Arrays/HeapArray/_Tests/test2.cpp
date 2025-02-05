// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

// Project headers
# include "../HeapArray.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../TestHelpers/TestHelpers.h"

class EmplaceTwo
{
    public:
        EmplaceTwo(const std::string& name, const int number) : m_name(name), m_number(number), m_present(m_name + " " + TestHelpers::to_string(m_number)) {};
		~EmplaceTwo() {};
		
        bool operator==(const EmplaceTwo& other)
        {
            return (m_name == other.m_name && m_number == other.m_number);
        }

        bool operator!=(const EmplaceTwo& other)
        {
            return (!(*this == other));
        }
        
		const std::string& present() const { return m_present; }

        std::string m_name;
        int         m_number;
		std::string m_present;
};

int TestPart2(int testNumber)
{
    try
	{
		TEST_INTRO(testNumber++);
		std::vector<int> 		std;
		HeapArray<int> 			array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}

        EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

		HeapArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
            EXPECT_EQUAL(*it == *iter, true, "value mismatch");
		}

        HeapArray<int> assign(100);
		
		assign.push_back(123);
		assign.push_back(-456);
		
        assign = array;

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy assignment value mismatch");
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
		std::vector<std::string> 		std;
		HeapArray<std::string> 			array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back("big string the will require allocation on the heap " + TestHelpers::to_string(i));
			array.emplace_back("big string the will require allocation on the heap " + TestHelpers::to_string(i));
		}
        EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

		HeapArray<std::string>::iterator it = array.begin();
		std::vector<std::string>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
            EXPECT_EQUAL(*it == *iter, true, "value mismatch");
		}

        HeapArray<std::string> assign(100);
		assign.push_back("big string the will require allocation on the heap ");
		assign.push_back("big string the will require allocation on the heap ");

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
		std::vector<int> 		std;
		HeapArray<int> 			array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}

        EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

		HeapArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
            EXPECT_EQUAL(*it == *iter, true, "value mismatch");
		}

        HeapArray<int> assign(array);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy, constructor value mismatch");
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
		std::vector<int> 		std;
		HeapArray<int> 			array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}

        EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

		HeapArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
            EXPECT_EQUAL(*it == *iter, true, "value mismatch");
		}

        HeapArray<int> assign(100);

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

        assign.clear();

        EXPECT_EQUAL(assign.size(), 0, "::clear failed, array not empty");

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    try
	{
		TEST_INTRO(testNumber++);
		std::vector<EmplaceTwo> 		std;
		HeapArray<EmplaceTwo> 			array(100);
		
		for (int i = 0; i < 2; ++i)
		{
			std.push_back(EmplaceTwo("name", i));
			array.push_back(EmplaceTwo("name", i));
		}

        EXPECT_EQUAL(std.size(), array.size(), "size mismatch");

		HeapArray<EmplaceTwo>::iterator it = array.begin();
		std::vector<EmplaceTwo>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
            EXPECT_EQUAL(it->present(), iter->present(), "value mismatch");
		}

		TEST_PASSED;
	}
	catch (const std::exception& e)
	{
		TEST_FAILED_MSG(e.what());
	}

    return (testNumber);
}
