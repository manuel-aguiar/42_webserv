/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maindealloc.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:20:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/23 09:15:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SharedMemoryPoolDealloc.hpp" 

#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <iostream>

int main(void)
{
    /*MemoryPool<std::string> pool1;*/

    SharedMemoryPool<std::string> pool1 = SharedMemoryPool<std::string>::make_SharedPool();

    std::list<std::string, SharedMemoryPool<std::string> > list(pool1);

    for (int i = 0; i < 100; i++)
    {
        list.push_back("one");
        list.push_back("two");
        list.push_back("three");
        list.push_back("four");
    }

    std::cout << "all allocated" << std::endl;

    for (int i = 0; i < 100; i++)
    {
        list.pop_back();
        list.pop_back();
        list.pop_back();
        list.pop_back();
    }
    std::cout << "all poped" << std::endl;
}