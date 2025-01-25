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
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<int> std;
		DynArray<int> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		TestHelpers::assertEqual(std.size(), array.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		DynArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}

        DynArray<int> assign;
        assign = array;

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "copy assignment, value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), assign.size(), "copy assignment, size mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<int> std;
		DynArray<int> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		TestHelpers::assertEqual(std.size(), array.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		DynArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}

        DynArray<int> assign(array);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "copy constructor, value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), assign.size(), "copy constructor, size mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<int> std;
		DynArray<int> array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		TestHelpers::assertEqual(std.size(), array.size(), "size mismatch", __FILE__, __LINE__, __FUNCTION__);

		DynArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}

        DynArray<int> assign;

        assign.push_back(1);
        assign.push_back(2);

        assign.move(array);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "::move failed, value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), assign.size(), "::move failed, size mismatch", __FILE__, __LINE__, __FUNCTION__);

        TestHelpers::assertEqual(array.size(), (size_t)0, "::move failed, source array not empty", __FILE__, __LINE__, __FUNCTION__);

        array.push_back(1);
        array.push_back(2);

        TestHelpers::assertEqual(std.size(), assign.size(), "::move failed, the movedFrom array influenced the moveTo", __FILE__, __LINE__, __FUNCTION__);

        array.clear();

        TestHelpers::assertEqual(array.size(), (size_t)0, "::clear failed, array not empty", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

/******************************************************************** */
/* Emplace two */

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<EmplaceTwo> std;
		DynArray<EmplaceTwo> array;
		
		for (int i = 0; i < 2; ++i)
		{
			std.push_back(EmplaceTwo("name", i));
			array.push_back(EmplaceTwo("name", i));
		}

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}
