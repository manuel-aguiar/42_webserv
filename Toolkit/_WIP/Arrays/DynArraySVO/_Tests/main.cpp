/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:40:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/23 10:06:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <string>
#include "../DynArraySVO.hpp"
#include <vector>

#include "../../../MemoryPool/MemoryPool.h"

class ToolkitDummy
{
	public:
		ToolkitDummy() : value(0), m_data(new int [4]), _name("i am a string so long that deffinitely allocates memory on the heap myself")
		{
			//std::cout << "dummy constructor" << std::endl;
		};

        ToolkitDummy(int value) : value(0), m_data(new int [4]), _name("i am a string so long that deffinitely allocates memory on the heap myself")
        {
            //std::cout << "dummy parameter constructor" << std::endl;
            this->value = value;
        }
		~ToolkitDummy()
		{
			//std::cout << "dummy destroy" << std::endl;
			if (m_data)  delete [] m_data;
				m_data = NULL;
		};
		ToolkitDummy(const ToolkitDummy& other) : value(other.value), m_data(new int [4]), _name(other._name)
		{
			//std::cout << "dummy copy" << std::endl;
			std::memcpy(m_data, other.m_data, 4 * sizeof(int));
		};
		ToolkitDummy& operator=(const ToolkitDummy& other)
		{
			if (!m_data)
				m_data = new int [4];
			//std::cout << " dummy copy assignment" << std::endl;
            value = other.value;
            _name = other._name;
			std::memcpy(m_data, other.m_data, 4 * sizeof(int));
			return (*this);
		};
		bool operator==(const ToolkitDummy& other) {return (value == other.value && _name == other._name);};
		bool operator!=(const ToolkitDummy& other) {return !(value == other.value && _name == other._name);};
        const char* print()
        {
            return("dummy: hey there ");
        }

	private:

        int value;
		int* m_data;
        std::string _name;
};

class ToolkitBase {
public:
    ToolkitBase() : m_data(new int[4]), value(0), _name("Base i am a string so long that deffinitely allocates memory on the heap myself") {
        //std::cout << "Base constructor" << std::endl;
    }

    ToolkitBase(int value) : m_data(new int[4]), value(value), _name("Base i am a string so long that deffinitely allocates memory on the heap myself") {
        //std::cout << "Base parameter constructor" << std::endl;
    }

	bool operator==(const ToolkitBase& other) {return (value == other.value && _name == other._name);};
	bool operator!=(const ToolkitBase& other) {return !(value == other.value && _name == other._name);};
    virtual ~ToolkitBase() {
        //std::cout << "Base destroy" << std::endl;
        if (m_data)
        {
            delete[] m_data;
            m_data = NULL;
        }
    }

    ToolkitBase(const ToolkitBase& other) : m_data(new int[4]), value(other.value), _name(other._name) {
        //std::cout << "Base copy" << std::endl;
        std::memcpy(m_data, other.m_data, 4 * sizeof(int));
    }

    ToolkitBase& operator=(const ToolkitBase& other) {
        if (this != &other) {
            //std::cout << "Base copy assignment" << std::endl;
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
    int* 			m_data;
    int 			value;
    std::string 	_name;

};

// Derived class
class ToolkitDerived : public ToolkitBase {
public:
    ToolkitDerived() : ToolkitBase() {
        //std::cout << "Derived constructor" << std::endl;
		_name = "Derived i am a string so long that deffinitely allocates memory on the heap myself";
    }

    ToolkitDerived(int value) : ToolkitBase(value) {
        //std::cout << "Derived parameter constructor" << std::endl;
		_name = "Derived i am a string so long that deffinitely allocates memory on the heap myself";
    }

    ~ToolkitDerived() {
        //std::cout << "Derived destroy" << std::endl;
    }

    ToolkitDerived(const ToolkitDerived& other) : ToolkitBase(other) {
        //std::cout << "Derived copy" << std::endl;
    }

    ToolkitDerived& operator=(const ToolkitDerived& other) {
        if (this != &other) {
            //std::cout << "Derived copy assignment" << std::endl;
            ToolkitBase::operator=(other);
        }
        return *this;
    }

	bool operator==(const ToolkitDerived& other) {return (value == other.value && _name == other._name);};
	bool operator!=(const ToolkitDerived& other) {return !(value == other.value && _name == other._name);};

    void print()
    {
        std::cout << "Derived: " << value << std::endl;
    }
};


# include <vector>

int main()
{

	/******************* TEST 1 ************************/

	try
	{
		std::cout << "TEST 1: ";
		std::vector<int> 		std;
		DynArraySVO<int> 			array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArraySVO<int>::iterator it = array.begin();
		std::vector<int>::iterator iter = std.begin();
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
	}

	/******************* TEST 2 ************************/

	try
	{
		std::cout << "TEST 2: ";
		std::vector<ToolkitDummy> 		std;
		DynArraySVO<ToolkitDummy> 			array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArraySVO<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
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
	}

	/******************* TEST 3 ************************/

	try
	{
		std::cout << "TEST 3: ";
		std::vector<ToolkitDummy> 		std;
		DynArraySVO<ToolkitDummy> 		array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArraySVO<ToolkitDummy>::iterator it = array.begin();
		std::vector<ToolkitDummy>::iterator iter = std.begin();
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
	}


	Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096);

	/******************* TEST 4 ************************/

	try
	{
		std::cout << "TEST 4: ";
		Nginx_PoolAllocator<ToolkitDummy> alloc(pool);
		std::vector<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > 		std(alloc);
		DynArraySVO<ToolkitDummy, 10, Nginx_PoolAllocator<ToolkitDummy> > 		array(alloc);

		std.reserve(23);
		array.reserve(23);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArraySVO<ToolkitDummy, 10, Nginx_PoolAllocator<ToolkitDummy> >::iterator it = array.begin();
		std::vector<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator iter = std.begin();
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
	}

/******************* TEST 5 ************************/

	try
	{
		std::cout << "TEST 5: ";
		Nginx_PoolAllocator<ToolkitBase*> alloc(pool);
		std::vector<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> > 		std(alloc);
		DynArraySVO<ToolkitBase*, 10, Nginx_PoolAllocator<ToolkitBase*> > 			array(alloc);

		std.reserve(23);
		array.reserve(23);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(new ToolkitBase(i));
			array.push_back(new ToolkitBase(i));

			std.push_back(new ToolkitDerived(i));
			array.push_back(new ToolkitDerived(i));

			std.push_back(new ToolkitDerived(i));
			array.push_back(new ToolkitDerived(i));

			std.pop_back();
			array.pop_back();
			std.push_back(new ToolkitBase(i));
			array.push_back(new ToolkitBase(i));
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArraySVO<ToolkitBase*, 10, Nginx_PoolAllocator<ToolkitBase*> >::iterator it = array.begin();
		std::vector<ToolkitBase*, Nginx_PoolAllocator<ToolkitBase*> >::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (**it != **iter)
				throw std::logic_error("value mismatch");
		}
		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}



	pool->destroy();

/*

	Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096);
    std::cout << "              destroyed my memoryPool vector" << std::endl << std::endl;

    {
        std::cout << "          my memoryPool vector of polymorphic objects:" << std::endl;



        Nginx_PoolAllocator<Base*> alloc(pool);
        MyVectorInPool<Base*>::type* vec = (MyVectorInPool<Base*>::type*)pool->allocate(sizeof(MyVectorInPool<Base*>::type), sizeof(MyVectorInPool<Base*>::type));
        new (vec) MyVectorInPool<Base*>::type(alloc);

        vec->emplace_front(new Base(5));
        vec->emplace_front(new Base(6));
        vec->emplace_front(new Base(7));
        vec->emplace_front(new Derived(1));
        vec->emplace_front(new Derived(2));
        vec->emplace_front(new Derived(3));

        std::cout << "              printing vector, expecting: Der3, Der2, Der1, Base7, Base6, Base5" << std::endl;

        for (MyVectorInPool<Base*>::type::iterator iter = vec->begin(); iter != vec->end(); ++iter)
        {
            (*iter)->print();
        }

        for (MyVectorInPool<Base*>::type::iterator iter = vec->begin(); iter != vec->end(); ++iter)
        {
            delete (*iter);
        }

        vec->~DynArraySVO();
		std::cout << "vec ouyt" << std::endl;
    }
	pool->destroy();

    std::cout << "              destroyed my memoryPool vector" << std::endl << std::endl;

  */



    return (0);
}

/*
    c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && ./main
*/
