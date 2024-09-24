/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainNoPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:20:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/24 09:49:00 by mmaria-d         ###   ########.fr       */
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

    std::list<std::string> list1;
    std::list<std::string> list2;
    std::list<std::string> list3;

    for (int i = 0; i < times; i++)
    {
        list1.push_back("one");
        list1.push_front("two");
        list2.push_back("three");
        list3.push_back("four");
        list2.push_front("five");
        list1.push_back("three");
        list1.pop_front();  // Removes "two"
        list1.pop_front();  // Removes "one"
        list1.push_back("four");
        
        list3.push_back("six");
        list1.push_back("seven");
        list2.push_back("eight");
        list2.pop_back();   // Removes "eight"
        list3.pop_front();  // Removes "four"
        list1.push_front("nine");
        list1.pop_back();   // Removes "four"
        list1.push_back("ten");
        list2.push_front("eleven");
        list2.pop_front();  // Removes "eleven"
        list3.push_back("twelve");
        
        list1.push_front("thirteen");
        list2.push_back("fourteen");
        list1.pop_front();  // Removes "thirteen"
        list3.push_back("fifteen");
        list3.pop_back();   // Removes "fifteen"
        list2.push_front("sixteen");
        list1.push_back("seventeen");
        list1.pop_back();   // Removes "seventeen"
        list1.push_back("eighteen");
        list3.push_front("nineteen");
        list2.pop_front();  // Removes "sixteen"
        list3.push_back("twenty");
    }

    return (0);
    //std::cout << "all poped" << std::endl;
}