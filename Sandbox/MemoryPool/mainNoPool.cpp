/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainNoPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:20:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/23 14:21:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <iostream>

int main(void)
{
    /*MemoryPool<std::string> pool1;*/


    std::list<std::string> list;

    for (int i = 0; i < 100; i++)
    {
        list.push_back("one");
        list.push_back("two");
        list.push_back("three");
        list.pop_back();
        list.pop_back();
        list.push_back("four");
    }

    //std::cout << "all allocated" << std::endl;
    //for(std::list<std::string, SharedMemoryPool<std::string> >::iterator iter = list.begin(); iter != list.end(); ++iter)
    //    std::cout << *iter << std::endl;
    for (int i = 0; i < 100; i++)
    {
        list.pop_back();
        list.pop_back();
    }
    //std::cout << "all poped" << std::endl;
}