/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:14:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/25 11:33:50 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SharedMPool_Dealloc.hpp" 

#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <iostream>

int main(void)
{
    {
        std::list<int, MemoryPool_Dealloc<int> > list1; // list with its own default pool
        std::list<int, MemoryPool_Dealloc<int> > list2; // list with its own default pool
    }


    {
        
            MemoryPool_Dealloc<int> pool1; // okay;
            MemoryPool_Dealloc<int> pool2(4096, 3, 5); // okay, page size 4096, 3 starting blocks, 5 spare blocks
             // this is okay
       

            //std::list<int, MemoryPool_Dealloc<int> > list5(pool2); // THIS IS UNFORTUNATELLY OKAY
            //std::list<int, MemoryPool_Dealloc<int> > list6(pool2); // THIS IS UNFORTUNATELLY OKAY
         /*    They are sharing the same pool, not what we want, will lead to undefined behavior, leaks and double frees    */

            std::list<int, MemoryPool_Dealloc<int> > list1(MemoryPool_Dealloc<std::string>()); // THIS IS UNFORTUNATELLY OKAY
            std::list<int, MemoryPool_Dealloc<int> > list2; //this is okay

            std::list<int, MemoryPool_Dealloc<int> > list3(MemoryPool_Dealloc<int>()); // okay, list with its own pool

            std::list<int, MemoryPool_Dealloc<int> > list4(MemoryPool_Dealloc<int>(4096, 2, 5)); // this is okay


    }
    {
        std::list<int, SharedMPool_Dealloc<int> > list1; // list with its pool
        std::list<int, SharedMPool_Dealloc<int> > list2(list1.get_allocator()); // sharing the pool with somebody else

        list1.push_back(123);
        list2.push_back(345);
    }
    
    {
        SharedMPool_Dealloc<int> pool1; // okay;
        SharedMPool_Dealloc<int> pool2(4096, 3, 5); // okay, page size 4096, 3 starting blocks, 5 spare blocks

        std::list<int, SharedMPool_Dealloc<int> > list1(pool1); // this is okay
        std::list<int, SharedMPool_Dealloc<int> > list2(pool1); // this is okay
        std::list<int, SharedMPool_Dealloc<int> > list3(pool2); // this is okay
        std::list<int, SharedMPool_Dealloc<int> > list4(pool2); // this is okay

        //std::list<int, SharedMPool_Dealloc<int> > list5(SharedMPool_Dealloc<int>()); // THIS IS NOT OKAY

        SharedMPool_Dealloc<int> pool3;
        std::list<int, SharedMPool_Dealloc<int> > list6(pool3); // this is okay
    }


    return (0);
}