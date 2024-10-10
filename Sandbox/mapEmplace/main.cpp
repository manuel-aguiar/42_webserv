/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 14:27:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/10 10:39:48 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <map>
#include <cstring>

class Dummy
{
	public:
		Dummy() : value(0), m_data(new int [4]), _name("i am a string so long that deffinitely allocates memory on the heap myself")
		{
			std::cout << "dummy constructor" << std::endl;
		};

        Dummy(int value) : value(0), m_data(new int [4]), _name("i am a string so long that deffinitely allocates memory on the heap myself")
        {
            std::cout << "dummy parameter constructor" << std::endl;
            this->value = value;
        }
		~Dummy()
		{
			std::cout << "dummy destroy" << std::endl;
			if (m_data)  delete [] m_data;
				m_data = NULL;
		};
		Dummy(const Dummy& other) : value(other.value), m_data(new int [4]), _name(other._name)
		{
			std::cout << "dummy copy" << std::endl;
			std::memcpy(m_data, other.m_data, 4 * sizeof(int));
		};
		Dummy& operator=(const Dummy& other)
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
            std::cout << "              Dummy: " << value << std::endl;
        }

	private:

        int value;
		int* m_data;
        std::string _name;
};

class Base {
public:
    Base() : m_data(new int[4]), value(0), _name("i am a string so long that deffinitely allocates memory on the heap myself") {
        std::cout << "Base constructor" << std::endl;
    }

    Base(int value) : m_data(new int[4]), value(value), _name("i am a string so long that deffinitely allocates memory on the heap myself") {
        std::cout << "Base parameter constructor" << std::endl;
    }

    virtual ~Base() {
        std::cout << "Base destroy" << std::endl;
        if (m_data)
        {
            delete[] m_data;
            m_data = NULL;
        }
    }

    Base(const Base& other) : m_data(new int[4]), value(other.value), _name(other._name) {
        std::cout << "Base copy" << std::endl;
        std::memcpy(m_data, other.m_data, 4 * sizeof(int));
    }

    Base& operator=(const Base& other) {
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
class Derived : public Base {
public:
    Derived() : Base() {
        std::cout << "Derived constructor" << std::endl;
    }

    Derived(int value) : Base(value) {
        std::cout << "Derived parameter constructor" << std::endl;
    }

    ~Derived() {
        std::cout << "Derived destroy" << std::endl;
    }

    Derived(const Derived& other) : Base(other) {
        std::cout << "Derived copy" << std::endl;
    }

    Derived& operator=(const Derived& other) {
        if (this != &other) {
            std::cout << "Derived copy assignment" << std::endl;
            Base::operator=(other);
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
	std::map<int, SharedPtr<Dummy> > map;

	for (int i = 0; i < 10000;++i)
		map.insert(std::pair<int, SharedPtr<Dummy> >(i, make_SharedPtr<Dummy, int, std::allocator<Dummy> >(i)));

	return (0);
}

int main3()
{

	//super copying is faster than shared pointer

	std::map<int, Dummy> map;

	for (int i = 0; i < 10000;++i)
		map.insert(std::pair<int, Dummy>(i, Dummy(i)));

	return (0);
}


int main(void)
{
    Nginx_MemoryPool*                   pool = Nginx_MemoryPool::create(4096);
    Nginx_PoolAllocator<char>           allocChar(pool);
    Nginx_PoolAllocator<StringInPool>   allocString(pool);





    std::cout  << "              testing map insert, just pair" << std::endl;
    {
        std::map<int, StringInPool, std::less<int>,  Nginx_PoolAllocator<StringInPool>  > map(std::less<int>(), allocString);
		StringInPool str("super long string that happens to be bigger than internal buffer", allocChar);

        map.insert(std::pair<int, StringInPool>(1, str));
    }


	pool->destroy();

    return (0);
}

/*

  c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../Webserv_sketch/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../Webserv_sketch/MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main
*/
