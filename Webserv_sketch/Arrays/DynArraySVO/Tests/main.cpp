/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:40:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 12:49:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <string>
#include "../DynArraySVO.hpp"
#include <vector>

#include "../../../MemoryPool/MemoryPool.h"

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

int main()
{
    /*
    {
        DynArray<Dummy> vec;
        vec.reserve(5);
        std::cout << " reserved 5 (allocated but didn't construct anything)" << std::endl;
        std::cout << "      pushin back" << std::endl;
        vec.push_back(Dummy());

        std::cout << "      pushed back (copied, constructed, destroy the copy)" << std::endl;
        
    }
    std::cout << "destroyed first vector" << std::endl << std::endl;


    {
        std::cout << "                  new vector, no reserve" << std::endl;
        DynArray<Dummy> vec2;
        std::cout << "                  new vector, done" << std::endl;
        vec2.push_back(Dummy());
        std::cout << "                  pushed back (allocate, copied, constructed, destroy the copy)" << std::endl;
    }

    std::cout << "destroyed second vector" << std::endl << std::endl;

    {
        std::cout << "                  new vector, reserve 5" << std::endl;    
        DynArray<Dummy> vec3;
        vec3.reserve(5);
        std::cout << "                  new vector, done, allocated for 5, constructed nothing" << std::endl;
        std::cout << "                  emplacing back, default constructor" << std::endl;
        vec3.emplace_back();
        std::cout << "                  emplaced back (constructed, destroyed, no copies)" << std::endl;
    }

    std::cout << "destroyed third vector" << std::endl;
     */  
    
    {
        std::cout << "          std::vector:" << std::endl;

        std::vector<Dummy> vec;
        vec.push_back(5);
    }
    std::cout << "              destroyed standard vector" << std::endl << std::endl;
    {
        std::cout << "          myvector:" << std::endl;

        DynArraySVO<Dummy, 10> vec;
        vec.emplace_back(5);
        vec.emplace_back(5);
        vec.emplace_back(5);
        vec.emplace_back(5);
        vec.emplace_back(5);
        vec.emplace_back(5);
        
    }
    std::cout << "              destroyed myvector" << std::endl << std::endl;

    {
        std::cout << "          my memoryPool vector:" << std::endl;

        Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096);

        Nginx_PoolAllocator<Dummy> alloc(pool);
        MyVectorSVOInPool<Dummy, 1>::type* vec = (MyVectorSVOInPool<Dummy, 1>::type*)pool->allocate(sizeof(MyVectorSVOInPool<Dummy, 1>::type), sizeof(MyVectorSVOInPool<Dummy, 1>::type));
        new (vec) MyVectorSVOInPool<Dummy, 1>::type(alloc);

        vec->emplace_back(5);
        vec->emplace_back(5);
        vec->emplace_back(5);
        vec->emplace_back(5);
        vec->emplace_back(5);
        vec->emplace_back(5);
        vec->emplace_back(5);
        vec->emplace_back(5);

        vec->~DynArraySVO();
        pool->destroy();
    }

    
    std::cout << "              destroyed my memoryPool vector" << std::endl << std::endl;

    {
        std::cout << "          my memoryPool vector of polymorphic objects:" << std::endl;

        Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096);

        Nginx_PoolAllocator<Base*> alloc(pool);

        MyVectorSVOInPool<Base*, 1>::type* 
        vec =  (MyVectorSVOInPool<Base*, 1>::type*)
            pool->allocate(
                    sizeof(MyVectorSVOInPool<Base*, 1>::type), 
                    sizeof(MyVectorSVOInPool<Base*, 1>::type)
        );
        new (vec) MyVectorSVOInPool<Base*, 1>::type(alloc);

        vec->emplace_front(new Base(5));
        vec->emplace_front(new Base(6));
        vec->emplace_front(new Base(7));
        vec->emplace_front(new Derived(1));
        vec->emplace_front(new Derived(2));
        vec->emplace_front(new Derived(3));

        std::cout << "              printing vector, expecting: Der3, Der2, Der1, Base7, Base6, Base5" << std::endl;

        for (MyVectorSVOInPool<Base*, 1>::type::iterator iter = vec->begin(); iter != vec->end(); ++iter)
        {
            (*iter)->print();
        }

        std::cout << "              remove back, insert front" << std::endl;

        delete (vec->back());
        vec->pop_back();
        delete (vec->back());
        vec->pop_back();
        vec->emplace_front(new Base(8));
        vec->emplace_front(new Base(9));

        std::cout << "              printing vector, expecting: Bse9, Bse8 Der3, Der2, Der1, Base7" << std::endl;

        for (MyVectorSVOInPool<Base*, 1>::type::iterator iter = vec->begin(); iter != vec->end(); ++iter)
        {
            (*iter)->print();
        }

        for (MyVectorSVOInPool<Base*, 1>::type::iterator iter = vec->begin(); iter != vec->end(); ++iter)
        {
            delete (*iter);
        }

        vec->~DynArraySVO();
        pool->destroy();
    }

    
    std::cout << "              destroyed my memoryPool vector" << std::endl << std::endl;


    return (0);
}

int main1()
{
    DynArraySVO<Dummy, 2> vec;

    vec.push_front(5);
    vec.push_front(6);
    vec.push_front(7);

    return (0);
}

/*
    c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && ./main
*/