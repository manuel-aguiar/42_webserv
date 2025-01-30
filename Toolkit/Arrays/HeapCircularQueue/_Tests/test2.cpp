// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

// Project headers
# include "../HeapCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../TestHelpers/TestHelpers.h"

class EmplaceTwo
{
    public:
        EmplaceTwo(const std::string& name, const int number) : m_name(name), m_number(number), m_present (std::string(m_name) + " " + TestHelpers::to_string(m_number)) {};
        ~EmplaceTwo() {};
        
        bool operator==(const EmplaceTwo& other)
        {
            return (m_name == other.m_name && m_number == other.m_number);
        }

        bool operator!=(const EmplaceTwo& other)
        {
            return (!(*this == other));
        }
        
        const std::string& present() const
        {
            return (m_present);
        }
        
        std::string m_name;
        int         m_number;
        std::string m_present;
};

std::ostream& operator<<(std::ostream& os, const EmplaceTwo& emplace)
{
    os << "Name: " << emplace.m_name << " Number: " << emplace.m_number;
    return os;
}

int TestPart2(int testNumber)
{
/******************************************************************** */

    try
    {
        std::cout << "TEST " << testNumber << ": ";
        std::vector<int> std;
        HeapCircularQueue<int> queue(100);

        for (int i = 0; i < 100; ++i)
        {
            std.push_back(i);
            queue.emplace_back(i);
        }

        EXPECT_EQUAL(std.size(), queue.size(), "size mismatch");

        HeapCircularQueue<int>::iterator it = queue.begin();
        std::vector<int>::iterator iter = std.begin();

        for (; it != queue.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch");
        }

        HeapCircularQueue<int> assign(100);
        assign.push_front(123);
        assign.push_front(456);
        
        assign = queue;

        EXPECT_EQUAL(queue.size(), assign.size(), "size mismatch after copy assignment");
        EXPECT_EQUAL(std.size(), assign.size(), "size mismatch after copy assignment");

        it = assign.begin();
        iter = std.begin();

        for (; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy assignment value mismatch");
        }

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }
    testNumber++;

/******************************************************************** */

    try
    {
        std::cout << "TEST " << testNumber << ": ";
        std::vector<int> std;
        HeapCircularQueue<int> queue(100);

        for (int i = 0; i < 100; ++i)
        {
            std.push_back(i);
            queue.push_back(i);
        }

        EXPECT_EQUAL(std.size(), queue.size(), "size mismatch");

        HeapCircularQueue<int>::iterator it = queue.begin();
        std::vector<int>::iterator iter = std.begin();
        for (; it != queue.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch");
        }

        HeapCircularQueue<int> assign(100);
        assign.push_back(123);
        assign.push_back(234);

        assign = queue;

        EXPECT_EQUAL(std.size(), assign.size(), "size mismatch after copy assignment");

        it = assign.begin();
        iter = std.begin();
        
        for (; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy assignment value mismatch");
        }

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }
    testNumber++;

/******************************************************************** */

    try
    {
        std::cout << "TEST " << testNumber << ": ";
        std::vector<std::string> std;
        HeapCircularQueue<std::string> queue(100);

        for (int i = 0; i < 100; ++i)
        {
            std.push_back("big string the will require allocation on the heap " + TestHelpers::to_string(i));
            queue.emplace_back("big string the will require allocation on the heap " + TestHelpers::to_string(i));
        }

        EXPECT_EQUAL(std.size(), queue.size(), "size mismatch");

        HeapCircularQueue<std::string>::iterator it = queue.begin();
        std::vector<std::string>::iterator iter = std.begin();
        for (; it != queue.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch");
        }

        HeapCircularQueue<std::string> assign(100);
        assign.push_back("big string on the heap but is different from the original");
        assign.push_back("big string on the heap but is different from the original");

        assign = queue;

        it = assign.begin();
        iter = std.begin();

        for (; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy assignment value mismatch");
        }

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }
    testNumber++;

/******************************************************************** */

    try
    {
        std::cout << "TEST " << testNumber << ": ";
        std::vector<int> std;
        HeapCircularQueue<int> queue(100);

        for (int i = 0; i < 100; ++i)
        {
            std.push_back(i);
            queue.emplace_back(i);
        }

        EXPECT_EQUAL(std.size(), queue.size(), "size mismatch");

        HeapCircularQueue<int>::iterator it = queue.begin();
        std::vector<int>::iterator iter = std.begin();
        for (; it != queue.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "value mismatch");
        }

        HeapCircularQueue<int> assign(queue);

        it = assign.begin();
        iter = std.begin();

        for (; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(*it, *iter, "copy constructor value mismatch");
        }

        EXPECT_EQUAL(std.size(), assign.size(), "copy constructor size mismatch");

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }
    testNumber++;

/******************************************************************** */
/* Emplace two */

    try
    {
        std::cout << "TEST " << testNumber << ": ";
        std::vector<EmplaceTwo> std;
        HeapCircularQueue<EmplaceTwo> queue(100, EmplaceTwo("name", 0));

        for (int i = 0; i < 2; ++i)
        {
            std.push_back(EmplaceTwo("name", i));
            queue.push_back(EmplaceTwo("name", i));
        }

        EXPECT_EQUAL(std.size(), queue.size(), "size mismatch");

        HeapCircularQueue<EmplaceTwo>::iterator it = queue.begin();
        std::vector<EmplaceTwo>::iterator iter = std.begin();
        for (; it != queue.end() && iter != std.end(); ++it, ++iter)
        {
            EXPECT_EQUAL(it->present(), iter->present(), "value mismatch");
        }

        std::cout << "	PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "	FAILED: " << e.what()  << std::endl;
    }
    testNumber++;

    return (testNumber);
}
