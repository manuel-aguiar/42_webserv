/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:57:10 by manuel            #+#    #+#             */
/*   Updated: 2024/10/10 12:42:21 by manuel           ###   ########.fr       */
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

int main(void)
{
/*
	{
		std::cout << "			my list: " << std::endl;
		List<Dummy> list;

		list.push_back(1);

		std::cout << "			finished" << std::endl;
	}

	{
		std::cout << "			std::list: " << std::endl;
		std::list<Dummy> list;
		list.push_back(1);
		std::cout << "			finished" << std::endl;
	}

	{
		std::cout << "			my list EMNPLACE BACK: " << std::endl;
		List<Dummy> list;

		list.emplace_back(1);
		list.emplace_back(1);
		list.emplace_back(1);
		list.emplace_back(1);

		List<Dummy>::iterator it = list.begin();
		(void)it;



		std::cout << "			finished" << std::endl;
	}
*/

	Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096);

/*
	{
		std::cout << "			my list EMNPLACE BACK: " << std::endl;
		Nginx_PoolAllocator<Dummy> alloc(pool);
		List<Dummy, Nginx_PoolAllocator<Dummy> > list(alloc);

		list.emplace_back(1);
		list.emplace_back(1);
		list.emplace_back(1);
		list.emplace_back(1);

		List<Dummy, Nginx_PoolAllocator<Dummy> >::iterator it = list.begin();
		(void)it;

		std::cout << "			finished" << std::endl;
	}
*/
    Nginx_PoolAllocator<char>           allocChar(pool);	//pool allocator for std::string character arrays
    Nginx_PoolAllocator<StringInPool>   allocString(pool);	//pool allocator for the std::string iself


    std::cout  << "              List of string mempool" << std::endl;
    {
        List<StringInPool, Nginx_PoolAllocator<StringInPool> > list(allocString);
		list.emplace_front("super long string that happens to be bigger than internal buffer", allocChar);
		list.emplace_front("super long sasga happens to be bigger than internal buffer", allocChar);
		list.emplace_front("super long string that happens to be bigger than internal buffer", allocChar);
		list.emplace_front("super longsgasg string that happens to be bigger than internal buffer", allocChar);
		list.emplace_front("super long string that happens to be bigger than internal buffer", allocChar);
		list.emplace_front("super long string that happens to be bigger than internal buffer", allocChar);
		list.emplace_front("super long string thatasgasgigger than internal buffer", allocChar);
		list.emplace_front("super long asg be bigger than internal buffer", allocChar);
		list.emplace_front("super long stasggaagsgasgaagagto be bigger than internal buffer", allocChar);

		for (List<StringInPool, Nginx_PoolAllocator<StringInPool> >::iterator it = list.begin(); it != list.end(); ++it)
		{
			std::cout << "              " << it->c_str() << std::endl;
		}
	}

	std::cout << "			finished" << std::endl;
	pool->destroy();


	return (0);
}

/*
    c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && ./main
*/
