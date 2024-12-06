/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:41:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 15:37:32 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../MemoryPool.h"
#include <list>
#include <iostream>

int main()
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
}


/*

clear && rm -rf vgcore* && c++ -Wall -Wextra -Werror *.cpp ../Nginx_MemoryPool.cpp ../Nginx_MPool_Block.cpp -g --std=c++98 && valgrind ./a.out
*/
