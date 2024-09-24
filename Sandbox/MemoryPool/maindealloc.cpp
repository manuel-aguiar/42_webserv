/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maindealloc.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:20:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/24 12:58:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SharedMemoryPoolDealloc.hpp" 

#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>

int main1(void)
{
    std::cout << "                  listtest" << std::endl;         

    SharedMemoryPool<std::string> pool1 = SharedMemoryPool<std::string>::make_SharedPool();
    
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


    std::cout << "                  map test" << std::endl; 
    SharedMemoryPool<std::pair<int, std::string> > pool2 = SharedMemoryPool<std::pair<int, std::string> >::make_SharedPool();
    std::less<int> comparator;

    typedef std::map<int, std::string, std::less<int>, SharedMemoryPool<std::pair<int, std::string> > >             PoolMap;
    typedef std::map<int, std::string, std::less<int>, SharedMemoryPool<std::pair<int, std::string> > >::iterator   PoolMap_iter;

    

    PoolMap map1(comparator, pool2);
    PoolMap map2(comparator, pool2);
    
    for ( int i = 0; i < 5; i++)
    {
        map1[i] = "map1 one";
        map2[i] = "map2 two";
    }


    for(PoolMap_iter iter = map1.begin(); iter != map1.end(); ++iter)
        std::cout << iter->first << ": " << iter->second << std::endl;

    for(PoolMap_iter iter = map2.begin(); iter != map2.end(); ++iter)
        std::cout << iter->first << ": " << iter->second << std::endl;

    std::cout << "                  set test" << std::endl; 


    SharedMemoryPool<std::string> pool4 = SharedMemoryPool<std::string>::make_SharedPool();
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

int main(int ac, char **av)
{
    /*MemoryPool<std::string> pool1;*/

    if (ac != 2)
        return (0);
    int times = std::atoi(av[1]);

    SharedMemoryPool<std::string> pool1 = SharedMemoryPool<std::string>::make_SharedPool();
    std::list<std::string, SharedMemoryPool<std::string> > list1(pool1);
    std::list<std::string, SharedMemoryPool<std::string> > list2(pool1);
    std::list<std::string, SharedMemoryPool<std::string> > list3(pool1);

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

    std::cout << "                                                              ended, now destructors"<<    std::endl;

    return (0);
    //std::cout << "all poped" << std::endl;
}