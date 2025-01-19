

#include "../SharedMPool_AlignDealloc.hpp" 

#include <map>
#include <set>
#include <string>
#include <vector>
#include <list>
#include <iostream>

int main(void)
{
    {
        std::list<int, MemoryPool_AlignDealloc<int> > list1; // list with its own default pool
        std::list<int, MemoryPool_AlignDealloc<int> > list2; // list with its own default pool
    }


    {
        
            MemoryPool_AlignDealloc<int> pool1; // okay;
            MemoryPool_AlignDealloc<int> pool2(4096, 3, 5); // okay, page size 4096, 3 starting blocks, 5 spare blocks
             // this is okay
       

            //std::list<int, MemoryPool_AlignDealloc<int> > list5(pool2); // THIS IS UNFORTUNATELLY OKAY
            //std::list<int, MemoryPool_AlignDealloc<int> > list6(pool2); // THIS IS UNFORTUNATELLY OKAY
         /*    They are sharing the same pool, not what we want, will lead to undefined behavior, leaks and double frees    */

            std::list<int, MemoryPool_AlignDealloc<int> > list1(MemoryPool_AlignDealloc<std::string>()); // THIS IS UNFORTUNATELLY OKAY
            std::list<int, MemoryPool_AlignDealloc<int> > list2; //this is okay

            std::list<int, MemoryPool_AlignDealloc<int> > list3(MemoryPool_AlignDealloc<int>()); // okay, list with its own pool

            std::list<int, MemoryPool_AlignDealloc<int> > list4(MemoryPool_AlignDealloc<int>(4096, 2, 5)); // this is okay


    }
    {
        std::list<int, SharedMPool_AlignDealloc<int> > list1; // list with its pool
        std::list<int, SharedMPool_AlignDealloc<int> > list2(list1.get_allocator()); // sharing the pool with somebody else

        list1.push_back(123);
        list2.push_back(345);
    }
    
    {
        SharedMPool_AlignDealloc<int> pool1; // okay;
        SharedMPool_AlignDealloc<int> pool2(4096, 3, 5); // okay, page size 4096, 3 starting blocks, 5 spare blocks

        std::list<int, SharedMPool_AlignDealloc<int> > list1(pool1); // this is okay
        std::list<int, SharedMPool_AlignDealloc<int> > list2(pool1); // this is okay
        std::list<int, SharedMPool_AlignDealloc<int> > list3(pool2); // this is okay
        std::list<int, SharedMPool_AlignDealloc<int> > list4(list2.get_allocator()); // this is okay

        list1.push_back(123);
        list2.push_back(345);
        list3.push_back(567);
        list4.push_back(789);
        list1.push_back(123);
        list2.push_back(345);
        list3.push_back(567);
        list4.push_back(789);

        
        //std::list<int, SharedMPool_AlignDealloc<int> > list5(SharedMPool_AlignDealloc<int>()); // THIS IS NOT OKAY

        SharedMPool_AlignDealloc<int> pool3;
        std::list<int, SharedMPool_AlignDealloc<int> > list6(pool3); // this is okay
    }


    return (0);
}