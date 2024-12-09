/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:41:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/09 14:21:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../MemoryPool.h"
#include "../../../../List/List.hpp"
#include <list>
#include <iostream>

int main(void)
{
	std::cout << "MY list with standard allcoator\n";
	List<int>	list0;

	list0.push_back(2);
	list0.push_back(2);
	list0.push_back(2);
	list0.push_back(2);
	list0.push_back(2);
	list0.push_back(2);
	list0.push_back(2);
	list0.push_back(2);

	for (List<int>::iterator iter = list0.begin(); iter != list0.end(); ++iter)
		std::cout << "alloc address: " << &(*iter) << '\n';

	std::cout << "\nMY list with custom allcoator\n";
	Nginx_MemoryPool *pool = Nginx_MemoryPool::create(4096, 1);
	List<int, Nginx_MPool_FixedElem<int> > list1(Nginx_MPool_FixedElem<int>(pool, 20));

	list1.push_back(2);
	list1.push_back(2);
	list1.push_back(2);
	list1.push_back(2);
	list1.push_back(2);
	list1.push_back(2);
	list1.push_back(2);
	list1.push_back(2);

	for (List<int, Nginx_MPool_FixedElem<int> >::iterator iter = list1.begin(); iter != list1.end(); ++iter)
		std::cout << "alloc address: " << &(*iter) << '\n';

	list1.clear();

	std::cout << "\nstd::list list with standard allcoator\n";
	std::list<int>	list2;

	list2.push_back(2);
	list2.push_back(2);
	list2.push_back(2);
	list2.push_back(2);
	list2.push_back(2);
	list2.push_back(2);
	list2.push_back(2);
	list2.push_back(2);

	for (std::list<int>::iterator iter = list2.begin(); iter != list2.end(); ++iter)
		std::cout << "alloc address: " << &(*iter) << '\n';

	std::cout << "\nstd::list list with custom allcoator\n";
	std::list<int, Nginx_MPool_FixedElem<int> > list3(Nginx_MPool_FixedElem<int>(pool, 20));

	list3.push_back(2);
	list3.push_back(2);
	list3.push_back(2);
	list3.push_back(2);
	list3.push_back(2);
	list3.push_back(2);
	list3.push_back(2);
	list3.push_back(2);

	for (std::list<int>::iterator iter = list3.begin(); iter != list3.end(); ++iter)
		std::cout << "alloc address: " << &(*iter) << '\n';

	list3.clear();
	pool->destroy();


/*
	std::list<int>	list01;

	list01.push_back(2);
	list01.push_back(2);
	list01.push_back(2);
	list01.push_back(2);

	for (std::list<int>::iterator iter = list01.begin(); iter != list01.end(); ++iter)
	{
		std::cout << &(*iter) << "\n";
		std::cout << sizeof(std::list<int>::value_type) << "\n";
	}

	Nginx_MemoryPool *pool = Nginx_MemoryPool::create(4096, 1);

	std::list<int, Nginx_MPool_FixedElem<int> > list1(Nginx_MPool_FixedElem<int>(pool, 20));

    list1.push_back(1);

	std::cout << "\nmylist\n\n";
	List<int, Nginx_MPool_FixedElem<int> > list2(Nginx_MPool_FixedElem<int>(pool, 20));

	std::cout << sizeof(std::list<int>) << "   " << sizeof(list1) << "   " << sizeof(list2) << std::endl;

    list2.push_back(1);

    list1.push_back(2);
    list1.push_back(3);
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);
    list1.push_back(1);
    list1.push_back(2);
    list1.push_back(3);   
    list1.push_back(3); 
    list1.pop_front();
    list1.pop_front();
    list1.pop_front();
    list1.pop_front();
    list1.push_back(3); 
    list1.pop_front();
    list1.push_back(3);   
    list1.push_back(3);
    list1.pop_front();
    list1.pop_front();
    list1.pop_front();
    list1.push_back(3);   
    list1.push_back(3);

	list1.clear();
	list2.clear();

	pool->destroy();
*/


	return (0);
}


int main1()
{
	Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096, 1);


	Nginx_PoolAllocator<std::string> alloc(pool);

	DynArray<std::string, Nginx_PoolAllocator<std::string> > vec((Nginx_PoolAllocator<std::string>(pool)));

	vec.reserve(1000);

/*	//Nginx_MPool_FixedElem<std::string> alloc2(pool, 123);

	std::cout<< "pool alive\n";

	//std::list<std::string, MPool_FixedElem<std::string> > listed(alloc2);
	//listed.push_back("cenas");

	//list.resize(100);
	//std::list<std::string, Nginx_MPool_FixedElem<std::string> > elems(alloc2);

	DynArray<std::string, Nginx_PoolAllocator<std::string> > elems(12, alloc);
	DynArray<std::string, Nginx_PoolAllocator<std::string> > elems2(12, alloc);

	elems.clear();
	elems2.clear();
	//elems.push_back("tretas");
*/

	std::cout << "list time" << "\n";

/*	*/
	//std::list<std::string, Nginx_MPool_FixedElem<std::string> > elem3(alloc2);

	// extra set of parenthesis to avoid dumb deductions...........................
	std::list<std::string, Nginx_MPool_FixedElem<std::string> > elem3((Nginx_MPool_FixedElem<std::string>(pool, 20)));
	
	elem3.push_back("cenas");

	std::cout << "list ready\n";


	
	elem3.clear();

	pool->destroy();

	return (0);
}


/*

clear && rm -rf vgcore* && c++ -Wall -Wextra -Werror *.cpp ../Nginx_MemoryPool.cpp ../Nginx_MPool_Block.cpp -g --std=c++98 && valgrind ./a.out
*/