/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:41:38 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/22 18:29:47 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Memory_Pool.hpp"

#include <map>
#include <string>
#include <vector>
#include <list>


typedef std::map<int, std::string, std::less<int>, MemoryPool<std::pair<int, std::string> > > PoolMap;
typedef std::map<int, std::string, std::less<int>, MemoryPool<std::pair<int, std::string> > >::iterator PoolMap_iter;


int main(void)
{
/*
    MemoryPool<std::pair<int, std::string> > pool;
    std::less<int> comparator;
    PoolMap map1;
    PoolMap map2;

    map1[1] = "map1 one";

    map1[2] = "map1 two";
    map2[1] = "map2 one";
    map2[2] = "map2 two";

    for (PoolMap_iter it = map1.begin(); it != map1.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    for (PoolMap_iter it = map2.begin(); it != map2.end(); ++it)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
*/

    SharedMemoryPool<std::string, 4096> pool = make_SharedPool<std::string, 4096>();
    
    typedef std::list<std::string, SharedMemoryPool<std::string> >::iterator PoolList_iter;

    std::list<std::string, SharedMemoryPool<std::string> > vec(pool);
std::list<std::string, SharedMemoryPool<std::string> > vec2(pool);
std::list<std::string, SharedMemoryPool<std::string> > vec3(pool);

    std::cout << "heyhey" << std::endl; 

    vec.push_back("one");
    vec.push_back("two");
    vec.push_back("three");
    vec.push_back("four");
    vec2.push_back("one");
    vec2.push_back("two");
    vec3.push_back("one");
    vec3.push_back("two");

    for(PoolList_iter iter = vec.begin(); iter != vec.end(); ++iter)
        std::cout << *iter << std::endl;



/*
    SharedMemoryPool<std::pair<int, std::string> > pool = make_SharedPool<std::pair<int, std::string>, 4096 >();
    std::less<int> comparator;

    typedef std::map<int, std::string, std::less<int>, SharedMemoryPool<std::pair<int, std::string> > > PoolMap;

    PoolMap map1(comparator, pool);
*/
    return (0);
}