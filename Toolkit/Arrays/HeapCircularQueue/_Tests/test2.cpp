

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

int StressTest(int testNumber)
{

/******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::vector<int> 		std;
		HeapCircularQueue<int> 			queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(queue.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapCircularQueue<int>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();

		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, \ngot '" + TestHelpers::to_string(*it) + "'\n expected: '" + TestHelpers::to_string(*iter) + "'\n"
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        HeapCircularQueue<int> assign(100);
		assign.push_front(123);
		assign.push_front(456);
		
        assign = queue;

		if (queue.size() != assign.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(queue.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (std.size() != assign.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(std.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy assignment, value mismatch, \ngot '" + TestHelpers::to_string(*it) + "'\n expected: '" + TestHelpers::to_string(*iter) + "'\n"
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch, got " + TestHelpers::to_string(assign.size()) + " expected: " + TestHelpers::to_string(std.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


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
		std::vector<int> 				std;
		HeapCircularQueue<int> 			queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.push_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(queue.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapCircularQueue<int>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, \ngot '" + TestHelpers::to_string(*it) + "'\n expected: '" + TestHelpers::to_string(*iter) + "'\n"
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        HeapCircularQueue<int> assign(100);
		assign.push_back(123);
		assign.push_back(234);

        assign = queue;

		if (std.size() != assign.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(assign.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy assignment, value mismatch, \ngot '" + TestHelpers::to_string(*it) + "'\n expected: '" + TestHelpers::to_string(*iter) + "'\n"
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(queue.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


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
		std::vector<std::string> 				std;
		HeapCircularQueue<std::string> 			queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back("big string the will require allocation on the heap " + TestHelpers::to_string(i));
			queue.emplace_back("big string the will require allocation on the heap " + TestHelpers::to_string(i));
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(std.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapCircularQueue<std::string>::iterator it = queue.begin();
		std::vector<std::string>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, got '" + *it + "'\n expected: '" + *iter + "'\n"
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        HeapCircularQueue<std::string> assign(100);
		assign.push_back("big string on the heap but is different from the original");
		assign.push_back("big string on the heap but is different from the original");

        assign = queue;

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy assignment, value mismatch, \ngot '" + *it + "'\n expected: '" + *iter + "'\n"
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(queue.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


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
		std::vector<int> 		std;
		HeapCircularQueue<int> 			queue(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

		HeapCircularQueue<int>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, \ngot '" + TestHelpers::to_string(*it) + "'\n expected: '" + TestHelpers::to_string(*iter) + "'\n"
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        HeapCircularQueue<int> assign(queue);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy constructor, value mismatch, \ngot '" + TestHelpers::to_string(*it) + "'\n expected: '" + TestHelpers::to_string(*iter) + "'\n"
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy constructor, size mismatch");


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
		std::vector<EmplaceTwo> 		std;
		HeapCircularQueue<EmplaceTwo> 			queue(100, EmplaceTwo("name", 0));
		
		for (int i = 0; i < 2; ++i)
		{
			std.push_back(EmplaceTwo("name", i));
			queue.push_back(EmplaceTwo("name", i));

			/*
				Fails because std::string didn't malloc and the destructor thinks
				it still lives in the same place before the vector reallocated........

				Which means, my dynAqueue cannot reallocate using memmove because some underlying
				variable may be referencing itself, and then it would point to an invalid location
				(as a std::string that doesn't allocate memory has to do, pointing to itself, without
				move semantics it invalidates itself. so i have a problem with my current setup)

				It works if the string is long enough such that it pointes to a malloc'ed place.
				But as long as it points to itself, i have a problem.

				So, i cannot use memmove after all and do have to deep copy everything.
				All i get is emplace_back, everything else, it must be a c++98 vector.



				but then....... why is a std::string calling free on a buffer inside the stack.....????
				it is just checking if the buffer is deferent from its own address.....
			*/

		}

		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + TestHelpers::to_string(queue.size()) + " expected: " + TestHelpers::to_string(std.size())
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		HeapCircularQueue<EmplaceTwo>::iterator it = queue.begin();
		std::vector<EmplaceTwo>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, \ngot '" + it->present() + "'\n expected: '" + iter->present() + "'\n"
				+ TestHelpers::FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
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