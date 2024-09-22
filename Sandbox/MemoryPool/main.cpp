/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 09:41:38 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/22 10:29:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Memory_Pool.hpp"

#include <map>
#include <string>
#include <vector>

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
    MemoryPool<std::string> pool;
    
    std::vector<std::string, MemoryPool<std::string> > vec(pool);
    std::vector<std::string, MemoryPool<std::string> > vec2(pool);
    std::vector<std::string, MemoryPool<std::string> > vec3(pool);
    
    vec.push_back("one");
    vec.push_back("two");
    vec2.push_back("one");
    vec2.push_back("two");
    vec3.push_back("one");
    vec3.push_back("two");

    for(unsigned int i = 0; i < vec.size(); ++i)
        std::cout << vec[i] << std::endl;



    return (0);
}