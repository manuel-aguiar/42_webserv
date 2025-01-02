/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 09:07:05 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/02 09:15:01 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// C++ headers
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>

// Project headers
#include "../StackArray.hpp"
#include "../../../_Tests/ToolkitDummy.hpp"
#include "../../../_Tests/ToolkitBase.hpp"
#include "../../../_Tests/ToolkitDerived.hpp"
# include "../../../_Tests/test.h"

template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

class Character
{
    public:
        Character() : m_name("no name") {}
        Character(const std::string& name) : m_name(name) {}
        Character(const Character& other) : m_name(other.m_name) {}
        Character& operator=(const Character& other) { m_name = other.m_name; return *this; }
        ~Character() {}
        
        bool operator==(const Character& other) const { return m_name == other.m_name; }
        bool operator!=(const Character& other) const { return m_name != other.m_name; }
        std::string getName() const { return m_name; }

    private:
        std::string m_name;
};


int TestPart3(int testNumber)
{

    /************************************ */

    // just making sure std::string can delete itself, must be deep copied....
    
	try
	{
		std::cout << "TEST " << testNumber++ << ": ";
		std::vector<Character> 		std;
		StackArray<Character, 100> 	array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back("name " + to_string(i));
			array.emplace_back("name " + to_string(i));
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		StackArray<Character, 100>::iterator it = array.begin();
		std::vector<Character>::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (*it != *iter)
				throw std::logic_error("value mismatch");
		}

        StackArray<Character, 100> assign(10); // default construct 10 out of the 100 cap
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
    return (testNumber);
}