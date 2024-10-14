/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:20:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/14 14:11:57 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../List.hpp"
#include <list>
#include "../../MemoryPool/MemoryPool.h"

int main()
{
    std::cout << "\n\n\n  std list:" <<std::endl;
    {
        std::list<int, MPool_FixedElem<int> > std(MPool_FixedElem<int>(5));

        for (int i = 0; i < 2; ++i)
        {
            std.push_back(i);
        }
    }

    std::cout << "\n\n\n  my list:" <<std::endl;
    {
        List<int, MPool_FixedElem<int> > std(MPool_FixedElem<int>(5));

        for (int i = 0; i < 2; ++i)
        {
            std.push_back(i);
        }
    }
/*
    std::cout << "\n\n\n  my list:" <<std::endl;
    Nginx_MemoryPool* pool = Nginx_MemoryPool::create(4096);
    {
        Nginx_PoolAllocator<int> alloc(pool);
        List<int, Nginx_PoolAllocator<int> > std(alloc);

        for (int i = 0; i < 2; ++i)
        {
            std.push_back(i);
        }
    }

    pool->destroy();
*/

    return (0);
}

/*

c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && rm -rf vgcore* && valgrind ./main

*/