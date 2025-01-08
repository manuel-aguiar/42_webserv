/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 13:55:45 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 00:03:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include <exception>
# include <stdexcept>
# include <iostream>
# include <vector>
# include <list>
# include <set>

# include "../Nginx_MemoryPool.hpp"
# include "../Nginx_PoolAllocator.hpp"
# include "../Nginx_MPool_FixedElem/Nginx_MPool_FixedElem.tpp"
# include "../../../_Tests/test.h"

/*

    Short demo of the Nginx_MemoryPool and the Nginx_PoolAllocator

*/


int main(void)
{
    int exampleNumber = 1;

    std::cout << "\n\nExample " << exampleNumber++ << ", allocate a std::string inside the memoryPool: \n";
    {
        Nginx_MemoryPool pool(4096, 1);
        // allocate a std::string inside the memoryPool (not initialized);
        std::string* str = (std::string*)pool.allocate(sizeof(std::string));

        // initialize the string with placement new
        new (str) std::string("super big string that must call malloc internally to allocate the buffer");

        std::cout << *str << std::endl;

        /*
            The string's destructor is not called!!!!!
        */

       //call the destructor
        str->~basic_string();

        //let the pool deallocate itself by going out of scope
    }

    std::cout << "\n\nExample " << exampleNumber++ << ", allocate a std::string inside the memoryPool, and use the pool for its own internal buffer: \n";
    {
        Nginx_MemoryPool pool(4096, 1);

        /*
            create a pool allocator, to allow a container to use the memoryPool for its internal storage

            A pool allocator is a sort of "interface" between our memory pool and the container
            The pool allcoator implements all methods required by std::allocator (such that it can be used as a template argument)
            such as allocate, deallocate, construct, destroy, etc
            And internally, when "allocating", it simply askes the associated Nginx_MemoryPool to allocate for it :D
            The Nginx_MemoryPool has no template type, it is simply a group of byte blocks
        */
        Nginx_PoolAllocator<char> alloc(pool);

        // this is a std::string (derived from basic_string) that uses the pool allocator
        typedef std::basic_string<char, std::char_traits<char>, Nginx_PoolAllocator<char> > NginxString;

         // allocate a std::string inside the memoryPool (not initialized);
        NginxString* str = (NginxString*)pool.allocate(sizeof(NginxString));

        // initialize the string with placement new, pass to it the initial value and the allocator
        new (str) NginxString("super big string that must call malloc internally to allocate the buffer", alloc);

        std::cout << *str << std::endl;

        /* 
            you no longer need to call the string destructor, memorypool takes care of the internal buffer!!!!!!!!!!!
        */

        //str->~basic_string();

        /*
            So you are handling an http request, allocating strings and so on, on the pool, request is over......
        */


        // reset the pool, the memory is still allocated but all marked as free
        pool.reset();
    

        NginxString   anotherString("another string that will be allocated on the pool", alloc);
        NginxString   anotherString2("some other stuff, heap allocated pleasseeeeee", alloc);

        std::cout << '\n';

        std::cout << '\t' << anotherString << std::endl;
        std::cout << '\t' << anotherString2 << std::endl;
        
        /*
            That's it. Even though these latest strings will call their destructor (stack), it doesn't matter
            They are recycling memory
        */



       /*
            Everything goes out of scope, the pool is the last one, deallocates all memory, no leaks
       */

    }


    std::cout << "\n\nExample " << exampleNumber++ << ", a container of std::string, container allcoates on pool, and strings allocate on pool \n";
    {
        Nginx_MemoryPool pool(4096, 1);
        
        // this is a std::string (derived from basic_string) that uses the pool allocator
        typedef std::basic_string<char, std::char_traits<char>, Nginx_PoolAllocator<char> > NginxString;

        // 2 Pool allocators, one for NginxStrings, and one for char buffers
        Nginx_PoolAllocator<char>   allocChar(pool);
        Nginx_PoolAllocator<NginxString> allocString(pool);

        // a vector of NginxStrings, that uses the PoolAllocator 
        std::vector<NginxString, Nginx_PoolAllocator<NginxString> > vec    
        (
                10,                                                                     
                        //10 elements
                NginxString("biiiiig strings that are for sure malloc'ed", allocChar),  
                        //copy constructed from this one, that uses a PoolAllocator for chars
                allocString                                                           
                        //and the vector uses PoolAllocator of NginxStrings for the vector's internal storage array
        );
        
        std::basic_string<char, std::char_traits<char>, Nginx_PoolAllocator<char> > str1("Hello", allocChar);
        str1 += " World"; 

        //maybe modify the strings a little bit, make them smaller to avoid reallocating, and print
        for (size_t i = 0; i < vec.size(); ++i)
        {
            vec[i].erase(vec[i].size() - 3);
            vec[i] += " ";
            vec[i] += to_string(i).c_str();
            
            std::cout << vec[i];
            vec[i] = " potato";
            std::cout << vec[i] << std::endl; 
        }

        /*
            no need to deallocate the strings ourselves: they are constructed and destructed by std::vector
            So, vector destructor is called, strings are destructed, and vector is destructed
            Then, the pool destructor is called, and the memory is deallocated

            All of this just took one single call to operator new, and one single call to operator delete
            This is featured on the tests, i won't demonstrate that now :D
        
            In this particular case, emplace back comes in really handy because you don't need to construct on main
            and using pool memory only for it to be copied internally and use pool memory again

            With a vector, you can also call ::reserve(), it will ask the allocator (in our case, the PoolAllocator, 
            which in turn asks the Nginx_MemoryPool) to allocate memory but won't call any constructors, just 
            guaranteess that the capacity is already installed for future push_backs
        */
    }

    std::cout << "\n\nExample " << exampleNumber++ << ": using the memoryPool to allocate a fixed block, from which a list allcoates and deallcoates nodes \n";
    {
        Nginx_MemoryPool pool(4096, 1);

        /*
            create a fixed block, of size 10 for integers, inside the memorypool
            (this is actually a lie.... the block is allocated when the first element is pushed)
            (and the std::list will rebind the allocator to allocate full std::list nodes....., but we can live in teh lie for a bit)
            
            As the base Nginx_PoolAllocator, this serves as an "interface" between our generic memoryPool and the std::list
            Internally, our FixedElem block simply askes the associated Nginx_MemoryPool to allocate the fixed size,
            and then it keeps track of the slots as they are "allocated and freed" by the std::list
        */
        
        FixedBlock_PoolAllocator<int> alloc(pool, 10);

        // create a list of ints, that uses the fixed block allocator
        std::list<int, FixedBlock_PoolAllocator<int> > list(alloc);

        list.push_back(1);
        list.push_back(2);
        list.push_back(3);
        
        list.pop_front();
        
        list.push_back(4);
        list.push_front(5);
        
        list.pop_back();

        for (std::list<int, FixedBlock_PoolAllocator<int> >::iterator it = list.begin(); it != list.end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;

        /*
            This is shown in the tests, if you alloc things while the program is running, you are sure that the
            std::list will always have its nodes close together in that fixed size block, within the memory pool

            Works for complex types -> list calls allocator.construct, allocator.destruct, pool goes out of scope, etc
            all clean, no leaks


            HOWEVER, instead of using a std::list with a fixed size block underneath, consider a HeapCircularQueue
            and pass a PoolAllocator to it -> smaller "nodes", no dereferencing and just pointer arithmatic,
            all packed tight together, and allocate it in your pool with the rest of the data that is frequently accessed
            All in one.
        */
    }


    std::cout << "\n\nExample " << exampleNumber++ << ": UNDEFINED BEHAVIOUR, some notes and examples, see code line " << __LINE__ << ", final remarks \n";
    {
        Nginx_MemoryPool pool(4096, 1);

        /*

            Unfortunately, some particular uses result in undefined behavior simply by the way
            that containers allocate memory.

            For instance, a std::list<int> does not simply allocate ints: + it allocates a prev and a next pointer
            for each node

            A std::map<int, int> allocates a pair<int, int> for each node, but on top of that,
            it allocates the parent node*, left child* and right child*.

            They REBIND the allcoator we pass them
        
        */

        FixedBlock_PoolAllocator<int> alloc(pool, 10);

        // this will copy construct the FixedElem Pool, no problem because it doesn't allocate until the first allcoation
        std::list<int, FixedBlock_PoolAllocator<int> > list1(alloc);


        // this doesn't mean they are sharing........
        std::list<int, FixedBlock_PoolAllocator<int> > list2(alloc);
        // it will actually allocate another separate block of memory for its own nodes

        // So one could expect these to share the same block, but they aren't


        //here comes another block, separate AND OF DIFFERENT SIZE TO THAT OF A LIST NODE
        std::set<int, FixedBlock_PoolAllocator<int> > set(alloc);
        // nobody is sharing anything, they are all separate blocks

        //I had another allocator to accomplish this but also figured: "why?, the point is to exactly stick things together"
        // "and segregate what should not be together" So we don't have it and won't have.


        /******************************************** */

        // while you can do this..... it is pretty useless ofc, not to mention the block will be copied from above
        std::vector<int, FixedBlock_PoolAllocator<int> > vec(alloc);



        //do this instead!!!
        std::vector<int, Nginx_PoolAllocator<int> > vec2(10, 0, Nginx_PoolAllocator<int>(pool));

        // also works with DynArray, HeapArray......




        /*
        
            The point of this is not really to count how many times we called malloc and free because it is "expensive"
            The point is to organize data, to make sure that the data the must be close together, stays together
            Our program will allcoate memory all the time, from different places and intermediate states kept
            on every EventPool loop.

            By using memory pools, we can force each connection to ahve it's state close together and not intermingled
            with data from other connections during runtime. So when we pull data from memory to cache to work on a connection
            we maximize the probability of actually using the data we are pulling in and not some other connection's data
            that the processor will have to discard because we will not use it.

            That is the MAIN reason for all of this.
        
        */
    }

    return (0);
}