/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:20:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/14 15:26:31 by mmaria-d         ###   ########.fr       */
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

    {
        List<int> list1;
        List<int> list2;

        list1.push_back(1);
        list1.push_back(2);
        list1.push_back(3);

        list2.push_back(4);
        list2.push_back(5);
        list2.push_back(6);
        list2.push_back(7);
        list2.push_back(8);


        List<int>::iterator it = list1.begin();
        ++it;

        List<int>::iterator it1 = list2.begin();
        ++it1;
        List<int>::iterator it2 = list2.end();
        

        list1.splice(it, list2, it1, it2);


        std::cout << "      list1" <<std::endl;
        it = list1.begin();
        while (it != list1.end())
        {
            std::cout << *it << std::endl;
            ++it;
        }

        std::cout << "      list2" <<std::endl;
        it = list2.begin();
        while (it != list2.end())
        {
            std::cout << *it << std::endl;
            ++it;
        }
    }

    return (0);
}

/*

c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.cpp ../../MemoryPool/Nginx_MemoryPool/Nginx_MPool_Block.cpp -o main && rm -rf vgcore* && valgrind ./main

*/