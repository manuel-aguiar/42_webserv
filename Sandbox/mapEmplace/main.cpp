/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:27:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 10:06:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <cstring>

class ToolkitDummy
{
	public:
		ToolkitDummy() : value(0), m_data(new int [4]), _name("i am a string so long that deffinitely allocates memory on the heap myself")
		{
			std::cout << "dummy constructor" << std::endl;
		};

        ToolkitDummy(int value) : value(0), m_data(new int [4]), _name("i am a string so long that deffinitely allocates memory on the heap myself")
        {
            std::cout << "dummy parameter constructor" << std::endl;
            this->value = value;
        }
		~ToolkitDummy()
		{
			std::cout << "dummy destroy" << std::endl;
			if (m_data)  delete [] m_data;
				m_data = NULL;
		};
		ToolkitDummy(const ToolkitDummy& other) : value(other.value), m_data(new int [4]), _name(other._name)
		{
			std::cout << "dummy copy" << std::endl;
			std::memcpy(m_data, other.m_data, 4 * sizeof(int));
		};
		ToolkitDummy& operator=(const ToolkitDummy& other)
		{
			if (!m_data)
				m_data = new int [4];
			std::cout << " dummy copy assignment" << std::endl;
            value = other.value;
            _name = other._name;
			std::memcpy(m_data, other.m_data, 4 * sizeof(int));
			return (*this);
		};

        void print()
        {
            std::cout << "              ToolkitDummy: " << value << std::endl;
        }

	private:

        int value;
		int* m_data;
        std::string _name;
};

class ToolkitBase {
public:
    ToolkitBase() : m_data(new int[4]), value(0), _name("i am a string so long that deffinitely allocates memory on the heap myself") {
        std::cout << "Base constructor" << std::endl;
    }

    ToolkitBase(int value) : m_data(new int[4]), value(value), _name("i am a string so long that deffinitely allocates memory on the heap myself") {
        std::cout << "Base parameter constructor" << std::endl;
    }

    virtual ~ToolkitBase() {
        std::cout << "Base destroy" << std::endl;
        if (m_data)
        {
            delete[] m_data;
            m_data = NULL;
        }
    }

    ToolkitBase(const ToolkitBase& other) : m_data(new int[4]), value(other.value), _name(other._name) {
        std::cout << "Base copy" << std::endl;
        std::memcpy(m_data, other.m_data, 4 * sizeof(int));
    }

    ToolkitBase& operator=(const ToolkitBase& other) {
        if (this != &other) {
            std::cout << "Base copy assignment" << std::endl;
            if (m_data) {
                delete[] m_data; // Clean up existing data
            }
            m_data = new int[4];
            std::memcpy(m_data, other.m_data, 4 * sizeof(int));
            value = other.value;
            _name = other._name;
        }
        return *this;
    }

    virtual void print()
    {
        std::cout << "Base: " << value << std::endl;
    }

protected:
    int* m_data;
    int value;
    std::string _name;

};

// Derived class
class ToolkitDerived : public ToolkitBase {
public:
    ToolkitDerived() : ToolkitBase() {
        std::cout << "Derived constructor" << std::endl;
    }

    ToolkitDerived(int value) : ToolkitBase(value) {
        std::cout << "Derived parameter constructor" << std::endl;
    }

    ~ToolkitDerived() {
        std::cout << "Derived destroy" << std::endl;
    }

    ToolkitDerived(const ToolkitDerived& other) : ToolkitBase(other) {
        std::cout << "Derived copy" << std::endl;
    }

    ToolkitDerived& operator=(const ToolkitDerived& other) {
        if (this != &other) {
            std::cout << "Derived copy assignment" << std::endl;
            ToolkitBase::operator=(other);
        }
        return *this;
    }

    void print()
    {
        std::cout << "Derived: " << value << std::endl;
    }
};

#include "../../Webserv_sketch/MemoryPool/MemoryPool.h"
#include "../../LibftCpp/libftcpp.hpp"
#include <map>
#include <list>

int main4()
{
	std::map<int, SharedPtr<ToolkitDummy> > map;

	for (int i = 0; i < 10000;++i)
		map.insert(std::pair<int, SharedPtr<ToolkitDummy> >(i, make_SharedPtr<ToolkitDummy, int, std::allocator<ToolkitDummy> >(i)));

	return (0);
}

int main3()
{

	//super copying is faster than shared pointer

	std::map<int, ToolkitDummy> map;

	for (int i = 0; i < 10000;++i)
		map.insert(std::pair<int, ToolkitDummy>(i, ToolkitDummy(i)));

	return (0);
}


int main(void)
{
    std::map<int, int> map1;
	std::map<std::string, std::string> map2;

	std::cout << "map1 sizeof: " << sizeof(map1) << "map2 sizeof: " << sizeof(map2) << std::endl;

    return (0);
}

/*

  c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../Webserv_sketch/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../Webserv_sketch/MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main
*/
