/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:57:10 by manuel            #+#    #+#             */
/*   Updated: 2024/12/23 10:06:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
#include "../../MemoryPool/MemoryPool.h"
#include "../List.hpp"
#include "../../_Tests/ToolkitDummy.hpp"

// C++ headers
#include <list>
#include <iostream>
#include <cstring>
#include <stdexcept>


int main(void)
{

	/******************* TEST 1 ************************/

	try
	{
		std::cout << "TEST 1: ";
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.push_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
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
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.push_front(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
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
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_front(i);
			list.emplace_front(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
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
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
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

		Nginx_PoolAllocator<ToolkitDummy> alloc(memoryPool);
		std::list<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > std(alloc);
		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> > list(alloc);

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			list.emplace_back(i);
		}
		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		List<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator it = list.begin();
		std::list<ToolkitDummy, Nginx_PoolAllocator<ToolkitDummy> >::iterator iter = std.begin();
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
		std::list<ToolkitDummy> 	std;
		List<ToolkitDummy> 		list;

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

		List<ToolkitDummy>::iterator it = list.begin();
		std::list<ToolkitDummy>::iterator iter = std.begin();
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
		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
		}

		List<ToolkitDummy> copy;

		copy = list;
		List<ToolkitDummy>::iterator iter = copy.begin();
		List<ToolkitDummy>::iterator it = list.begin();
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
		List<ToolkitDummy> 		copy;
		List<ToolkitDummy> 		list;

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
		List<ToolkitDummy>::iterator iter = copy.begin();
		List<ToolkitDummy>::iterator it = list.begin();

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

		List<ToolkitDummy> 		list;

		for (int i = 0; i < 100; ++i)
		{
			list.push_back(i);
		}

		List<ToolkitDummy> 		copy(list);
		List<ToolkitDummy>::iterator iter = copy.begin();
		List<ToolkitDummy>::iterator it = list.begin();
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
		List<ToolkitDummy> 		copy;
		List<ToolkitDummy> 		list;

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
		List<ToolkitDummy>::iterator iter = copy.begin();
		List<ToolkitDummy>::iterator it = list.begin();

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

/******************* TEST 16 ************************/

	try
	{
		std::cout << "TEST 16: ";


		std::list<int, Nginx_MPool_FixedElem<int> > 	std(Nginx_MPool_FixedElem<int>(memoryPool, 100));
		List<int, Nginx_MPool_FixedElem<int> > 			list(Nginx_MPool_FixedElem<int>(memoryPool, 100));

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list<int, Nginx_MPool_FixedElem<int> >::iterator iter = std.begin();
		List<int, Nginx_MPool_FixedElem<int> >::iterator it = list.begin();
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

/******************* TEST 17 ************************/

	try
	{
		std::cout << "TEST 17: ";


		std::list<int, Nginx_MPool_FixedElem<int> > 	std(Nginx_MPool_FixedElem<int>(memoryPool, 100));
		List<int, Nginx_MPool_FixedElem<int> > 			list(Nginx_MPool_FixedElem<int>(memoryPool, 100));

		for (int i = 0; i < 100; ++i)
		{
			std.push_back(i);
			std.splice(std.end(), std, std.begin());

			list.emplace_back(i);
			list.splice(list.end(), list, list.begin());
		}

		if (std.size() != list.size())
			throw std::logic_error("size mismatch");

		std::list<int, Nginx_MPool_FixedElem<int> >::iterator iter = std.begin();
		List<int, Nginx_MPool_FixedElem<int> >::iterator it = list.begin();
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
