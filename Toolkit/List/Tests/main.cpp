/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:57:10 by manuel            #+#    #+#             */
/*   Updated: 2024/12/09 09:17:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../List.hpp"
#include <list>

#include <cstring>


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

#include "../../MemoryPool/MemoryPool.h"
#include <stdexcept>

int main(void)
{

	/******************* TEST 1 ************************/

	try
	{
		std::cout << "TEST 1: ";
		std::list<Dummy> 	std;
		List<Dummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.push_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<Dummy>::iterator it = list.begin();
		std::list<Dummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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
		std::list<Dummy> 	std;
		List<Dummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.push_front(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<Dummy>::iterator it = list.begin();
		std::list<Dummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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
		std::cout << "TEST 3: ";
		std::list<Dummy> 	std;
		List<Dummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.emplace_front(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<Dummy>::iterator it = list.begin();
		std::list<Dummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	try
	{
		std::cout << "TEST 4: ";
		std::list<Dummy> 	std;
		List<Dummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<Dummy>::iterator it = list.begin();
		std::list<Dummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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


	Nginx_MemoryPool* memoryPool = Nginx_MemoryPool::create(4096);


	try
	{
		std::cout << "TEST 5: ";

		Nginx_PoolAllocator<Dummy> alloc(memoryPool);
		std::list<Dummy, Nginx_PoolAllocator<Dummy> > std(alloc);
		List<Dummy, Nginx_PoolAllocator<Dummy> > list(alloc);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<Dummy, Nginx_PoolAllocator<Dummy> >::iterator it = list.begin();
		std::list<Dummy, Nginx_PoolAllocator<Dummy> >::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	try
	{
		std::cout << "TEST 6: ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays
		Nginx_PoolAllocator<StringInPool>   allocString(memoryPool);

		std::list<StringInPool, Nginx_PoolAllocator<StringInPool> > std(allocString);
		List<StringInPool, Nginx_PoolAllocator<StringInPool> > 		list(allocString);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(StringInPool("super long string that happens to be bigger than internal buffer", allocChar));
			list.emplace_back("super long string that happens to be bigger than internal buffer", allocChar);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator it = list.begin();
		std::list<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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
		std::list<Dummy> 	std;
		List<Dummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.push_back(i);

			std.push_back(i + 100);
			list.push_back(i + 100);

			std.pop_front();
			list.pop_front();

			std.push_front(i + 100);
			list.push_front(i + 100);

			std.pop_back();
			list.pop_back();

			std.push_back(i + 100);
			list.push_back(i + 100);

			std.push_back(i + 100);
			list.push_back(i + 100);

			std.pop_front();
			list.pop_front();


		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<Dummy>::iterator it = list.begin();
		std::list<Dummy>::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	/******************* TEST 8 ************************/

	try
	{
		std::cout << "TEST 8: ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays
		Nginx_PoolAllocator<StringInPool>   allocString(memoryPool);

		std::list<StringInPool, Nginx_PoolAllocator<StringInPool> > std(allocString);
		List<StringInPool, Nginx_PoolAllocator<StringInPool> > 		list(allocString);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(StringInPool("cenas e coisas", allocChar));
			list.push_back(StringInPool("cenas e coisas", allocChar));

			std.push_back(StringInPool(" coisas", allocChar));
			list.push_back(StringInPool(" coisas", allocChar));

			std.pop_front();
			list.pop_front();

			std.push_front(StringInPool(" tretas coisas", allocChar));
			list.push_front(StringInPool(" tretas coisas", allocChar));

			std.pop_back();
			list.pop_back();

			std.push_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));
			list.push_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));

			std.push_back(StringInPool(" tretas coisas", allocChar));
			list.push_back(StringInPool(" tretas coisas", allocChar));

			std.pop_front();
			list.pop_front();


		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator it = list.begin();
		std::list<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator iter = std.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	/******************* TEST 9 ************************/

	try
	{
		std::cout << "TEST 9: ";
		List<Dummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
		}

		List<Dummy> copy;

		copy = list;
		List<Dummy>::iterator iter = copy.begin();
		List<Dummy>::iterator it = list.begin();
		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
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

	/******************* TEST 10 ************************/

	try
	{
		std::cout << "TEST 10: ";
		List<Dummy> 		copy;
		List<Dummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			copy.push_back(i + 1);
		}

		copy.push_back(1000);
		copy.push_back(1000);
		copy.push_back(1000);
		copy.push_back(1000);

		copy = list;
		List<Dummy>::iterator iter = copy.begin();
		List<Dummy>::iterator it = list.begin();

		if (list.size() != copy.size())
		{
			std::cout << "list size: " << list.size() << " copy size: " << copy.size() << std::endl;

			throw std::logic_error("size mismatch");
		}


		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
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

	/******************* TEST 11 ************************/

	try
	{
		std::cout << "TEST 11: ";

		List<Dummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
		}

		List<Dummy> 		copy(list);
		List<Dummy>::iterator iter = copy.begin();
		List<Dummy>::iterator it = list.begin();
		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
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

	/******************* TEST 12 ************************/

	try
	{
		std::cout << "TEST 12: ";
		List<Dummy> 		copy;
		List<Dummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
			copy.push_back(i + 1);
		}

		copy.pop_front();
		copy.pop_front();
		copy.pop_front();
		copy.pop_front();

		copy = list;
		List<Dummy>::iterator iter = copy.begin();
		List<Dummy>::iterator it = list.begin();

		if (list.size() != copy.size())
		{
			std::cout << "list size: " << list.size() << " copy size: " << copy.size() << std::endl;

			throw std::logic_error("size mismatch");
		}


		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
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

	/******************* TEST 13 ************************/

	try
	{
		std::cout << "TEST 13: ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays
		Nginx_PoolAllocator<StringInPool>   allocString(memoryPool);

		List<StringInPool, Nginx_PoolAllocator<StringInPool> > 		list(allocString);

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(StringInPool("cenas e coisas", allocChar));
			list.push_back(StringInPool(" coisas", allocChar));
			list.pop_front();
			list.push_front(StringInPool(" tretas coisas", allocChar));
			list.pop_back();
			list.push_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));
			list.push_back(StringInPool(" tretas coisas", allocChar));
			list.pop_front();
		}

		List<StringInPool, Nginx_PoolAllocator<StringInPool> > copy(list);

		if (copy.size() != list.size())
			throw std::logic_error("size mismatch");

		List<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator it = list.begin();
		List<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator iter = copy.begin();
		for ( ; it != list.end() && iter != copy.end(); ++it, ++iter)
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

/******************* TEST 14 ************************/

	try
	{
		std::cout << "TEST 14: ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays

		std::list<StringInPool, Nginx_MPool_FixedElem<StringInPool> > 	std((Nginx_MPool_FixedElem<StringInPool>(memoryPool, 5000)));
		List<StringInPool, Nginx_MPool_FixedElem<StringInPool> > 		list((Nginx_MPool_FixedElem<StringInPool>(memoryPool, 5000)));

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(StringInPool("cenas e coisas", allocChar));
			list.emplace_back(StringInPool("cenas e coisas", allocChar));

			std.push_back(StringInPool(" coisas", allocChar));
			list.emplace_back(StringInPool(" coisas", allocChar));

			std.pop_front();
			list.pop_front();

			std.push_front(StringInPool(" tretas coisas", allocChar));
			list.emplace_front(StringInPool(" tretas coisas", allocChar));

			std.pop_back();
			list.pop_back();

			std.push_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));
			list.emplace_front(StringInPool(" tretasfasfas coisasagsgasgasgs", allocChar));

			std.push_back(StringInPool(" tretas coisas", allocChar));
			list.emplace_back(StringInPool(" tretas coisas", allocChar));

			std.pop_front();
			list.pop_front();


		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list<StringInPool, Nginx_MPool_FixedElem<StringInPool> >::iterator iter = std.begin();
		List<StringInPool, Nginx_MPool_FixedElem<StringInPool> >::iterator it = list.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

/******************* TEST 15 ************************/

	try
	{
		std::cout << "TEST 15: ";

		Nginx_PoolAllocator<char>           allocChar(memoryPool);	//memoryPool allocator for std::string character arrays


		MPool_FixedElem<StringInPool> alloc(200);
		std::list<StringInPool, MPool_FixedElem<StringInPool> > std(alloc);
		List<StringInPool, MPool_FixedElem<StringInPool> > 		list(alloc);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(StringInPool("cenas e coisas", allocChar));
			list.push_back(StringInPool("cenas e coisas", allocChar));
		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list<StringInPool, MPool_FixedElem<StringInPool> >::iterator iter = std.begin();
		List<StringInPool, MPool_FixedElem<StringInPool> >::iterator it = list.begin();
		for ( ; it != list.end() && iter != std.end(); ++it, ++iter)
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

	memoryPool->destroy();

	return (0);
}

/*
c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && valgrind ./main
*/
