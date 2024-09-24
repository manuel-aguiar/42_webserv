/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainNoPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:20:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/24 14:57:38 by mmaria-d         ###   ########.fr       */
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

        // Additional 70 randomized operations
        list1.push_front("twenty-one");
        list2.push_back("twenty-two");
        list3.push_back("twenty-three");
        list1.pop_back();  // Removes "eighteen"
        list2.pop_back();  // Removes "twenty-two"
        list3.pop_back();  // Removes "twenty-three"

        list1.push_back("twenty-four");
        list3.push_back("twenty-five");
        list2.push_front("twenty-six");
        list1.pop_front();  // Removes "twenty-one"
        list3.pop_front();  // Removes "nineteen"
        list2.pop_back();   // Removes "three"

        list1.push_front("twenty-seven");
        list1.push_back("twenty-eight");
        list2.push_back("twenty-nine");
        list2.pop_front();  // Removes "twenty-six"
        list3.push_back("thirty");
        list1.pop_back();   // Removes "twenty-eight"

        list3.push_back("thirty-one");
        list1.push_back("thirty-two");
        list2.pop_back();  // Removes "twenty-nine"
        list1.push_front("thirty-three");
        list2.push_front("thirty-four");
        list3.pop_back();  // Removes "thirty-one"

        list3.push_back("thirty-five");
        list1.pop_back();   // Removes "thirty-two"
        list1.push_back("thirty-six");
        list2.push_back("thirty-seven");
        list2.pop_front();  // Removes "thirty-four"
        list1.push_front("thirty-eight");

        list2.push_front("thirty-nine");
        list3.push_back("forty");
        list2.push_back("forty-one");
        list3.pop_front();  // Removes "six"
        list1.pop_back();   // Removes "thirty-six"

        list2.push_back("forty-two");
        list3.push_front("forty-three");
        list1.pop_front();  // Removes "thirty-eight"
        list1.push_back("forty-four");
        list2.pop_front();  // Removes "thirty-nine"
        list3.pop_back();   // Removes "forty"

        list2.push_front("forty-five");
        list1.push_back("forty-six");
        list3.push_back("forty-seven");
        list2.pop_back();   // Removes "forty-two"
        list1.push_front("forty-eight");

        list1.pop_front();  // Removes "forty-eight"
        list1.push_back("forty-nine");
        list3.push_back("fifty");
        list2.push_back("fifty-one");
        list1.pop_back();   // Removes "forty-nine"
        list2.pop_back();   // Removes "fifty-one"

        list3.push_front("fifty-two");
        list1.push_front("fifty-three");
        list2.push_front("fifty-four");
        list1.pop_back();   // Removes "forty-six"
        list2.pop_front();  // Removes "fifty-four"

        list3.push_back("fifty-five");
        list1.push_back("fifty-six");
        list2.pop_back();   // Removes "three"
        list3.pop_back();   // Removes "fifty-five"
        list2.push_back("fifty-seven");
        list1.pop_front();  // Removes "fifty-three"

        list1.push_back("fifty-eight");
        list2.push_front("fifty-nine");
        list3.push_front("sixty");
        list1.pop_back();   // Removes "fifty-eight"
        list3.pop_back();   // Removes "fifty"

        list1.push_front("sixty-one");
        list2.push_back("sixty-two");
        list1.pop_back();   // Removes "fifty-six"
        list3.push_back("sixty-three");
        list2.pop_front();  // Removes "fifty-nine"
        list3.push_back("sixty-four");

        list2.push_front("sixty-five");
        list3.push_back("sixty-six");
        list1.push_back("sixty-seven");
        list1.pop_front();  // Removes "sixty-one"
        list3.pop_back();   // Removes "sixty-six"

        list1.push_front("sixty-eight");
        list3.push_back("sixty-nine");
        list2.pop_back();   // Removes "sixty-two"
        list1.push_back("seventy");
    }

    std::cout << "List 1: ";
    for (std::list<std::string>::iterator iter = list1.begin(); iter != list1.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;

    std::cout << "List 2: ";
    for (std::list<std::string>::iterator iter = list2.begin(); iter != list2.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;

    std::cout << "List 3: ";
    for (std::list<std::string>::iterator iter = list3.begin(); iter != list3.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;

    return (0);
    //std::cout << "all poped" << std::endl;
}