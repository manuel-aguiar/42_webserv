/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 09:14:40 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 14:31:29 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../SharedMPool_Alloc.hpp" 

#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <iostream>

int main(void)
{
    {
        std::list<int, MemoryPool_Alloc<int> > list1; // list with its own default pool
        std::list<int, MemoryPool_Alloc<int> > list2; // list with its own default pool
    }


    {
        
            MemoryPool_Alloc<int> pool1; // okay;
             // this is okay
       

            //std::list<int, MemoryPool_Alloc<int> > list5(pool2); // THIS IS UNFORTUNATELLY OKAY
            //std::list<int, MemoryPool_Alloc<int> > list6(pool2); // THIS IS UNFORTUNATELLY OKAY
         /*    They are sharing the same pool, not what we want, will lead to undefined behavior, leaks and double frees    */

            std::list<int, MemoryPool_Alloc<int> > list1(MemoryPool_Alloc<std::string>()); // THIS IS UNFORTUNATELLY OKAY
            std::list<int, MemoryPool_Alloc<int> > list2; //this is okay

            std::list<int, MemoryPool_Alloc<int> > list3(MemoryPool_Alloc<int>()); // okay, list with its own pool



    }
    {
        std::list<int, SharedMPool_Alloc<int> > list1; // list with its pool
        std::list<int, SharedMPool_Alloc<int> > list2(list1.get_allocator()); // sharing the pool with somebody else

        list1.push_back(123);
        list2.push_back(345);
    }
    
    {
        SharedMPool_Alloc<int> pool1; // okay;

        std::list<int, SharedMPool_Alloc<int> > list1(pool1); // this is okay
        std::list<int, SharedMPool_Alloc<int> > list2(pool1); // this is okay
        std::list<int, SharedMPool_Alloc<int> > list4(list2.get_allocator()); // this is okay

        list1.push_back(123);
        list2.push_back(345);
        list4.push_back(789);
        list1.push_back(123);
        list2.push_back(345);
        list4.push_back(789);

        
        //std::list<int, SharedMPool_Alloc<int> > list5(SharedMPool_Alloc<int>()); // THIS IS NOT OKAY

        SharedMPool_Alloc<int> pool3;
        std::list<int, SharedMPool_Alloc<int> > list6(pool3); // this is okay
    }


    return (0);
}