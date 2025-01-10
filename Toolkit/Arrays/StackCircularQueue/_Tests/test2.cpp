/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:22:17 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/10 09:38:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include "../../../_Tests/test.h"


class EmplaceTwo
{
    public:
        EmplaceTwo(const std::string& name, const int number) : m_name(name), m_number(number), m_present (std::string(m_name) + " " + to_string(m_number)) {};
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
		std::cout << "TEST " << testNumber++ << ": ";

		const int queueSize = 100;

		std::vector<int> 		std;
		StackCircularQueue<int, queueSize> 			queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + to_string(queue.size()) + " expected: " + to_string(queue.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackCircularQueue<int, queueSize>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();

		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, \ngot '" + to_string(*it) + "'\n expected: '" + to_string(*iter) + "'\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        StackCircularQueue<int, queueSize> assign;
		assign.push_front(123);
		assign.push_front(456);
		
        assign = queue;

		if (queue.size() != assign.size())
			throw std::logic_error("size mismatch, got " + to_string(assign.size()) + " expected: " + to_string(queue.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		if (std.size() != assign.size())
			throw std::logic_error("size mismatch, got " + to_string(assign.size()) + " expected: " + to_string(std.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy assignment, value mismatch, \ngot '" + to_string(*it) + "'\n expected: '" + to_string(*iter) + "'\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch, got " + to_string(assign.size()) + " expected: " + to_string(std.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


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

		std::vector<int> 				std;
		StackCircularQueue<int, queueSize> 			queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.push_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + to_string(queue.size()) + " expected: " + to_string(queue.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackCircularQueue<int, queueSize>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, \ngot '" + to_string(*it) + "'\n expected: '" + to_string(*iter) + "'\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        StackCircularQueue<int, queueSize> assign;
		assign.push_back(123);
		assign.push_back(234);

        assign = queue;

		if (std.size() != assign.size())
			throw std::logic_error("size mismatch, got " + to_string(queue.size()) + " expected: " + to_string(assign.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy assignment, value mismatch, \ngot '" + to_string(*it) + "'\n expected: '" + to_string(*iter) + "'\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch, got " + to_string(queue.size()) + " expected: " + to_string(queue.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


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

		std::vector<std::string> 							std;
		StackCircularQueue<std::string, queueSize> 			queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back("big string the will require allocation on the heap " + to_string(i));
			queue.emplace_back("big string the will require allocation on the heap " + to_string(i));
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch, got " + to_string(queue.size()) + " expected: " + to_string(std.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackCircularQueue<std::string, queueSize>::iterator it = queue.begin();
		std::vector<std::string>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, got '" + *it + "'\n expected: '" + *iter + "'\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        StackCircularQueue<std::string, queueSize> assign("i like potatos on the heap if possible");
		assign.push_back("big string on the heap but is different from the original");
		assign.push_back("big string on the heap but is different from the original");

        assign = queue;

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy assignment, value mismatch, \ngot '" + *it + "'\n expected: '" + *iter + "'\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch, got " + to_string(queue.size()) + " expected: " + to_string(queue.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));


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

		std::vector<int> 		std;
		StackCircularQueue<int, queueSize> 			queue;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			queue.emplace_back(i);
		}
		if (std.size() != queue.size())
			throw std::logic_error("size mismatch");

		StackCircularQueue<int, queueSize>::iterator it = queue.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, \ngot '" + to_string(*it) + "'\n expected: '" + to_string(*iter) + "'\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

        StackCircularQueue<int, queueSize> assign(queue);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy constructor, value mismatch, \ngot '" + to_string(*it) + "'\n expected: '" + to_string(*iter) + "'\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy constructor, size mismatch");


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

		const int queueSize = 100;

		std::vector<EmplaceTwo> 		std;
		StackCircularQueue<EmplaceTwo, queueSize> 			queue(EmplaceTwo("name", 0));
		
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
			throw std::logic_error("size mismatch, got " + to_string(queue.size()) + " expected: " + to_string(std.size())
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));

		StackCircularQueue<EmplaceTwo, queueSize>::iterator it = queue.begin();
		std::vector<EmplaceTwo>::iterator iter = std.begin();
		for ( ; it != queue.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch, \ngot '" + it->present() + "'\n expected: '" + iter->present() + "'\n"
				+ FileLineFunction(__FILE__, __LINE__, __FUNCTION__));
		}

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


    return (testNumber);
}