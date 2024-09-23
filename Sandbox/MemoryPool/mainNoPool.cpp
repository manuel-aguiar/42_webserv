/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainNoPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:20:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/23 20:50:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <cstdlib>

int main(int ac, char **av)
{
    /*MemoryPool<std::string> pool1;*/
    if (ac != 2)
        return (0);
    int times = std::atoi(av[1]);

    std::list<std::string> list;

    for (int i = 0; i < times; i++)
    {
        list.push_back("one");
        list.push_front("two");
        list.push_back("three");
        list.pop_front();
        list.pop_front();
        list.push_back("four");
    }

    //std::cout << "all allocated" << std::endl;
    //for(std::list<std::string, SharedMemoryPool<std::string> >::iterator iter = list.begin(); iter != list.end(); ++iter)
    //    std::cout << *iter << std::endl;
    for (int i = 0; i < times; i++)
    {
        list.pop_front();
        list.pop_back();
    }
    //std::cout << "all poped" << std::endl;
}