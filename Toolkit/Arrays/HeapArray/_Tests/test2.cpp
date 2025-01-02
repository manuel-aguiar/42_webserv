/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:22:17 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 10:23:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// Project headers
# include "../HeapArray.hpp"
# include "../../../_Tests/ToolkitDummy.hpp"
# include "../../../_Tests/ToolkitBase.hpp"
# include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/test.h"

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

/******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::vector<int> 		std;
		HeapArray<int> 			array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		HeapArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}

        HeapArray<int> assign(100);
        assign = array;

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy assignment, value mismatch");
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy assignment, size mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;

/******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::vector<int> 		std;
		HeapArray<int> 			array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		HeapArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}

        HeapArray<int> assign(array);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("copy constructor, value mismatch");
        }

        if (std.size() != assign.size())
			throw std::logic_error("copy constructor, size mismatch");


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;

/******************************************************************** */

    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::vector<int> 		std;
		HeapArray<int> 			array(100);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		HeapArray<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}

        HeapArray<int> assign(100);

        assign.push_back(1);
        assign.push_back(2);

        assign.move(array);

        it = assign.begin();
        iter = std.begin();
        
        for ( ; it != assign.end() && iter != std.end(); ++it, ++iter)
        {
            if (*it != *iter)
                throw std::logic_error("::move failed, value mismatch");
        }

        if (std.size() != assign.size())
			throw std::logic_error("::move failed, size mismatch");

        if (array.size() != 0)
            throw std::logic_error("::move failed, source array not empty");    

        array.clear();

        if (array.size() != 0)
            throw std::logic_error("::clear failed, array not empty");

		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
        TEST_FAIL_INFO();
	}
    testNumber++;

/******************************************************************** */
/* Emplace two */

    try
	{
		std::cout << "TEST " << testNumber << ": ";
		std::vector<EmplaceTwo> 		std;
		HeapArray<EmplaceTwo> 			array(100);
		
		for (int i = 0; i < 2; ++i)
		{
			std.push_back(EmplaceTwo("name", i));
			array.push_back(EmplaceTwo("name", i));

			/*
				Fails because std::string didn't malloc and the destructor thinks
				it still lives in the same place before the vector reallocated........

				Which means, my dynAarray cannot reallocate using memmove because some underlying
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

		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		HeapArray<EmplaceTwo>::iterator it = array.begin();
		std::vector<EmplaceTwo>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
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
    testNumber++;


    return (testNumber);
}