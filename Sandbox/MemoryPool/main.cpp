/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:41:38 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/22 19:03:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Memory_Pool.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>

int main(void)
{

    SharedMemoryPool<std::string, 4096> pool1 = make_SharedPool<std::string, 4096>();
    
    typedef std::list<std::string, SharedMemoryPool<std::string> >::iterator PoolList_iter;

    std::list<std::string, SharedMemoryPool<std::string> > list(pool1);
    std::list<std::string, SharedMemoryPool<std::string> > list2(pool1);
    std::list<std::string, SharedMemoryPool<std::string> > list3(pool1);

    std::cout << "heyhey" << std::endl; 

    list.push_back("one");
    list.push_back("two");
    list2.push_back("one");
    list2.push_back("two");
    list.push_back("three");
    list.push_back("four");

    list3.push_back("one");
    list3.push_back("two");
    list3.push_back("one");
    list3.push_back("two");
    list.push_back("one");
    list.push_back("two");
    
    for(PoolList_iter iter = list.begin(); iter != list.end(); ++iter)
        std::cout << *iter << std::endl;




    SharedMemoryPool<std::pair<int, std::string> > pool2 = make_SharedPool<std::pair<int, std::string>, 4096 >();
    std::less<int> comparator;

    typedef std::map<int, std::string, std::less<int>, SharedMemoryPool<std::pair<int, std::string> > >             PoolMap;
    typedef std::map<int, std::string, std::less<int>, SharedMemoryPool<std::pair<int, std::string> > >::iterator   PoolMap_iter;

    

    PoolMap map1(comparator, pool2);
    PoolMap map2(comparator, pool2);
    

    map1[1] = "map1 one";
    map1[2] = "map1 two";

    map2[1] = "map2 one";
    map2[2] = "map2 two";


    map1[3] = "map1 three";
    map1[4] = "map1 four";
    map2[3] = "map2 three";
    map2[4] = "map2 four";



    for(PoolMap_iter iter = map1.begin(); iter != map1.end(); ++iter)
        std::cout << iter->first << ": " << iter->second << std::endl;

    for(PoolMap_iter iter = map2.begin(); iter != map2.end(); ++iter)
        std::cout << iter->first << ": " << iter->second << std::endl;



    SharedMemoryPool<std::string, 4096> pool4 = make_SharedPool<std::string, 4096>();
    std::less<std::string> comparator4;

    typedef std::set<std::string, std::less<std::string>, SharedMemoryPool<std::string> >  PoolSet;
    typedef std::set<std::string, std::less<std::string>, SharedMemoryPool<std::string> >::iterator  PoolSet_iter;

    PoolSet set1(comparator4, pool4);

    set1.insert("set1 one");
    set1.insert("set2 two");

    for(PoolSet_iter iter = set1.begin(); iter != set1.end(); ++iter)
        std::cout << *iter << std::endl;

    return (0);
}