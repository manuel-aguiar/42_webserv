// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>

// Project headers
# include "../StackCircularQueue.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/TestHelpers.h"

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
		std::cout << "TEST " << testNumber++ << ": ";

		const int queueSize = 100;

		std::vector<int> std;
		StackCircularQueue<int, queueSize> queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		TestHelpers::assertEqual(std.size(), queue.size(), "Size mismatch in test 1", __FILE__, __LINE__, __FUNCTION__);

		StackCircularQueue<int, queueSize>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();

		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "Value mismatch", __FILE__, __LINE__, __FUNCTION__);
		}

        StackCircularQueue<int, queueSize> assign;
		assign.push_front(123);
		assign.push_front(456);
		
        assign = queue;

		TestHelpers::assertEqual(queue.size(), assign.size(), "Size mismatch after assignment", __FILE__, __LINE__, __FUNCTION__);
		TestHelpers::assertEqual(std.size(), assign.size(), "Size mismatch between std and assigned", __FILE__, __LINE__, __FUNCTION__);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "Copy assignment value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), assign.size(), "Copy assignment size mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    /******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		const int queueSize = 100;

		std::vector<int> std;
		StackCircularQueue<int, queueSize> queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.push_back(i);
		}
		TestHelpers::assertEqual(std.size(), queue.size(), "Size mismatch in test 2", __FILE__, __LINE__, __FUNCTION__);

		StackCircularQueue<int, queueSize>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();

		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "Value mismatch in test 2", __FILE__, __LINE__, __FUNCTION__);
		}

        StackCircularQueue<int, queueSize> assign;
		assign.push_back(123);
		assign.push_back(234);

        assign = queue;

		TestHelpers::assertEqual(queue.size(), assign.size(), "Size mismatch after assignment", __FILE__, __LINE__, __FUNCTION__);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "Copy assignment value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), assign.size(), "Copy assignment size mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    /******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		const int queueSize = 100;

		std::vector<std::string> std;
		StackCircularQueue<std::string, queueSize> queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back("big string the will require allocation on the heap " + TestHelpers::to_string(i));
			queue.emplace_back("big string the will require allocation on the heap " + TestHelpers::to_string(i));
		}
		TestHelpers::assertEqual(std.size(), queue.size(), "Size mismatch in string test", __FILE__, __LINE__, __FUNCTION__);

		StackCircularQueue<std::string, queueSize>::iterator it = queue.begin();
		std::vector<std::string>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "Value mismatch in string test", __FILE__, __LINE__, __FUNCTION__);
		}

        StackCircularQueue<std::string, queueSize> assign("i like potatoes on the heap if possible");
		assign.push_back("big string on the heap but is different from the original");
		assign.push_back("big string on the heap but is different from the original");

        assign = queue;

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "Copy assignment value mismatch in string test", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), assign.size(), "Copy assignment size mismatch in string test", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    /******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		const int queueSize = 100;

		std::vector<int> std;
		StackCircularQueue<int, queueSize> queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		TestHelpers::assertEqual(std.size(), queue.size(), "Size mismatch in test 4", __FILE__, __LINE__, __FUNCTION__);

		StackCircularQueue<int, queueSize>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(*it, *iter, "Value mismatch in test 4", __FILE__, __LINE__, __FUNCTION__);
		}

        StackCircularQueue<int, queueSize> assign(queue);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            TestHelpers::assertEqual(*it, *iter, "Copy constructor value mismatch", __FILE__, __LINE__, __FUNCTION__);
        }

        TestHelpers::assertEqual(std.size(), assign.size(), "Copy constructor size mismatch", __FILE__, __LINE__, __FUNCTION__);

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    /******************************************************************** */
    /* Emplace Two */

    try
	{
		std::cout << "TEST " << testNumber++ << ": ";

		const int queueSize = 100;

		std::vector<EmplaceTwo> std;
		StackCircularQueue<EmplaceTwo, queueSize> queue(EmplaceTwo("name", 0));
		
		for (int i = 0; i < 2; ++i)
		{
			std.push_back(EmplaceTwo("name", i));
			queue.push_back(EmplaceTwo("name", i));
		}

		TestHelpers::assertEqual(std.size(), queue.size(), "Size mismatch in EmplaceTwo test", __FILE__, __LINE__, __FUNCTION__);

		StackCircularQueue<EmplaceTwo, queueSize>::iterator it = queue.begin();
		std::vector<EmplaceTwo>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			TestHelpers::assertEqual(it->present(), iter->present(), "Value mismatch in EmplaceTwo test", __FILE__, __LINE__, __FUNCTION__);
		}

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}

    return (testNumber);
}
