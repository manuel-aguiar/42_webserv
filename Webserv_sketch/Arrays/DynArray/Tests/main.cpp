/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 08:40:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/11 08:35:55 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <string>
#include "../DynArray.hpp"
#include <vector>

#include "../../../MemoryPool/MemoryPool.h"

class Dummy
{
	public:
		Dummy() : value(0), m_data(new int [4]), _name("i am a string so long that deffinitely allocates memory on the heap myself")
		{
			//std::cout << "dummy constructor" << std::endl;
		};

        Dummy(int value) : value(0), m_data(new int [4]), _name("i am a string so long that deffinitely allocates memory on the heap myself")
        {
            //std::cout << "dummy parameter constructor" << std::endl;
            this->value = value;
        }
		~Dummy()
		{
			//std::cout << "dummy destroy" << std::endl;
			if (m_data)  delete [] m_data;
				m_data = NULL;
		};
		Dummy(const Dummy& other) : value(other.value), m_data(new int [4]), _name(other._name)
		{
			//std::cout << "dummy copy" << std::endl;
			std::memcpy(m_data, other.m_data, 4 * sizeof(int));
		};
		Dummy& operator=(const Dummy& other)
		{
			if (!m_data)
				m_data = new int [4];
			//std::cout << " dummy copy assignment" << std::endl;
            value = other.value;
            _name = other._name;
			std::memcpy(m_data, other.m_data, 4 * sizeof(int));
			return (*this);
		};
		bool operator==(const Dummy& other) {return (value == other.value && _name == other._name);};
		bool operator!=(const Dummy& other) {return !(value == other.value && _name == other._name);};
        const char* print()
        {
            return("dummy: hey there ");
        }

	private:

        int value;
		int* m_data;
        std::string _name;
};

class Base {
public:
    Base() : m_data(new int[4]), value(0), _name("Base i am a string so long that deffinitely allocates memory on the heap myself") {
        //std::cout << "Base constructor" << std::endl;
    }

    Base(int value) : m_data(new int[4]), value(value), _name("Base i am a string so long that deffinitely allocates memory on the heap myself") {
        //std::cout << "Base parameter constructor" << std::endl;
    }

	bool operator==(const Base& other) {return (value == other.value && _name == other._name);};
	bool operator!=(const Base& other) {return !(value == other.value && _name == other._name);};
    virtual ~Base() {
        //std::cout << "Base destroy" << std::endl;
        if (m_data)
        {
            delete[] m_data;
            m_data = NULL;
        }
    }

    Base(const Base& other) : m_data(new int[4]), value(other.value), _name(other._name) {
        //std::cout << "Base copy" << std::endl;
        std::memcpy(m_data, other.m_data, 4 * sizeof(int));
    }

    Base& operator=(const Base& other) {
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
class Derived : public Base {
public:
    Derived() : Base() {
        //std::cout << "Derived constructor" << std::endl;
		_name = "Derived i am a string so long that deffinitely allocates memory on the heap myself";
    }

    Derived(int value) : Base(value) {
        //std::cout << "Derived parameter constructor" << std::endl;
		_name = "Derived i am a string so long that deffinitely allocates memory on the heap myself";
    }

    ~Derived() {
        //std::cout << "Derived destroy" << std::endl;
    }

    Derived(const Derived& other) : Base(other) {
        //std::cout << "Derived copy" << std::endl;
    }

    Derived& operator=(const Derived& other) {
        if (this != &other) {
            //std::cout << "Derived copy assignment" << std::endl;
            Base::operator=(other);
        }
        return *this;
    }

	bool operator==(const Derived& other) {return (value == other.value && _name == other._name);};
	bool operator!=(const Derived& other) {return !(value == other.value && _name == other._name);};

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
		DynArray<int> 			array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<int>::iterator it = array.begin();
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
		std::vector<Dummy> 		std;
		DynArray<Dummy> 			array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.push_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<Dummy>::iterator it = array.begin();
		std::vector<Dummy>::iterator iter = std.begin();
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
		std::vector<Dummy> 		std;
		DynArray<Dummy> 			array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<Dummy>::iterator it = array.begin();
		std::vector<Dummy>::iterator iter = std.begin();
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
		Nginx_PoolAllocator<Dummy> alloc(pool);
		std::vector<Dummy, Nginx_PoolAllocator<Dummy> > 		std(alloc);
		DynArray<Dummy, Nginx_PoolAllocator<Dummy> > 			array(alloc);

		std.reserve(23);
		array.reserve(23);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			array.emplace_back(i);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<Dummy, Nginx_PoolAllocator<Dummy> >::iterator it = array.begin();
		std::vector<Dummy, Nginx_PoolAllocator<Dummy> >::iterator iter = std.begin();
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
		Nginx_PoolAllocator<Base*> alloc(pool);
		std::vector<Base*, Nginx_PoolAllocator<Base*> > 		std(alloc);
		DynArray<Base*, Nginx_PoolAllocator<Base*> > 			array(alloc);

		std.reserve(23);
		array.reserve(23);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(new Base(i));
			array.emplace_back(new Base(i));

			std.push_back(new Derived(i));
			array.emplace_back(new Derived(i));

			std.push_back(new Derived(i));
			array.emplace_back(new Derived(i));

			delete std.back();
			delete array.back();

			std.pop_back();
			array.pop_back();
			std.push_back(new Base(i));
			array.push_back(new Base(i));
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<Base*, Nginx_PoolAllocator<Base*> >::iterator it = array.begin();
		std::vector<Base*, Nginx_PoolAllocator<Base*> >::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (**it != **iter)
				throw std::logic_error("value mismatch");
		}

		it = array.begin();
		iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			delete (*it);
			delete (*iter);
		}


		std::cout << "	PASSED" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "	FAILED: " << e.what()  << std::endl;
	}


/******************* TEST 6 ************************/

	try
	{
		std::cout << "TEST 6: ";
		std::vector<Dummy> 		std;
		DynArray<Dummy> 			array;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.push_back(std[0]);
			array.emplace_back(i);
			array.emplace_back(array[0]);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<Dummy>::iterator it = array.begin();
		std::vector<Dummy>::iterator iter = std.begin();
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


/******************* TEST 7 ************************/

	try
	{
		std::cout << "TEST 7: ";
		Nginx_PoolAllocator<Base*> alloc(pool);
		std::vector<Base*, Nginx_PoolAllocator<Base*> > 		std(alloc);
		DynArray<Base*, Nginx_PoolAllocator<Base*> > 			array(alloc);

		std.reserve(23);
		array.reserve(23);
		Base* base;

		for (int i = 0; i < 100; ++i)
		{
						base = (Base *)pool->allocate(sizeof(Base), sizeof(Base));
						new (base) Base(i);
			std.push_back(base);

						base = (Base *)pool->allocate(sizeof(Base), sizeof(Base));
						new (base) Base(i);

			array.emplace_back(base);

						base = (Derived *)pool->allocate(sizeof(Derived), sizeof(Derived));
						new (base) Derived(i);

			std.push_back(base);

						base = (Derived *)pool->allocate(sizeof(Derived), sizeof(Derived));
						new (base) Derived(i);

			array.emplace_back(base);

						base = (Derived *)pool->allocate(sizeof(Derived), sizeof(Derived));
						new (base) Derived(i);

			std.push_back(base);

						base = (Derived *)pool->allocate(sizeof(Derived), sizeof(Derived));
						new (base) Derived(i);

			array.emplace_back(base);

			std.back()->~Base();
			array.back()->~Base();

			std.pop_back();

			array.pop_back();

						base = (Base *)pool->allocate(sizeof(Base), sizeof(Base));
						new (base) Base(i);
			std.push_back(base);

						base = (Base *)pool->allocate(sizeof(Base), sizeof(Base));
						new (base) Base(i);

			array.emplace_back(base);
		}
		if (std.size() != array.size())
			throw std::logic_error("size mismatch");

		DynArray<Base*, Nginx_PoolAllocator<Base*> >::iterator it = array.begin();
		std::vector<Base*, Nginx_PoolAllocator<Base*> >::iterator iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			if (**it != **iter)
				throw std::logic_error("value mismatch");
		}

		it = array.begin();
		iter = std.begin();
		for ( ; it != array.end() && iter != std.end(); ++it, ++iter)
		{
			(*it)->~Base();
			(*iter)->~Base();
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

        vec->~DynArray();
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
