/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:30:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/26 09:43:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*

    Fixed sized blocks. if the alloc is large, ask directly from the system using malloc
    Pool saves a pointer to the current empty block pool
    saves a list of

*/


#ifndef NGINX_MEMORYPOOL_HPP

# define NGINX_MEMORYPOOL_HPP

# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# define BLOCK_SIZE_MAX 4096


typedef unsigned char t_byte;

/*

    Nginx-style non deallocating pool

*/

class Nginx_MemoryPool
{
    public:

        Nginx_MemoryPool() {throw std::logic_error("Nginx_MemoryPool, can't instantiate out of the box, has to go through static .create()");}
        ~Nginx_MemoryPool() {}
        
        /*
        
            We add blocks to the top of the list, makes sense from a cache perspective (most recent block is the closest)
            and when adding new blocks, they will all come on top.
            Same for all other memory pools we have
            
            Same for single malloc blocks (big_blocks)
        
        */

        static Nginx_MemoryPool* create(size_t blockSize, size_t startingBlocks = 1)
        {
            Nginx_MemoryPool dummy(1);  //total dummy
            Nginx_MemoryPool* pool;
            Nginx_MemoryPool* next;

            next = NULL;
            for (size_t i = 0; i < startingBlocks - 1; ++i)
            {
                pool = _allocateNewBlock(blockSize);
                
                //new block is on top of the previous one
                pool->_nextBlock = next;
                next = pool;
            }
            return (pool);
        }

        static void destroy(Nginx_MemoryPool* pool)
        {
            t_bigBlock*         bigCur;
            t_bigBlock*         bigNext;
            Nginx_MemoryPool*   poolNext;

            while (pool)
            {
                poolNext = pool->_nextBlock;
                bigCur = pool->_bigBlocks;
                while (bigCur)
                {
                    bigNext = bigCur->_nextBlock;
                    delete [] (t_byte*)(bigCur->_mallocedBlock);
                    bigCur = bigNext;
                }
                delete [] (t_byte*)(pool);
                pool = poolNext;
            }
        }

        
        /*
        
            When resetting, delete the big blocks, we don't really know their size of use
            let the next pool user to take care of it
        
        */

        static void reset(Nginx_MemoryPool* pool)
        {
            t_bigBlock*         bigCur;
            t_bigBlock*         bigNext;
            Nginx_MemoryPool*   poolNext;

            while (pool)
            {
                poolNext = pool->_nextBlock;
                bigCur = pool->_bigBlocks;
                while (bigCur)
                {
                    bigNext = bigCur->_nextBlock;
                    delete [] (t_byte*)(bigCur->_mallocedBlock);
                    bigCur = bigNext;
                }
                pool->_bigBlocks = NULL;
                pool->_freePosition = (t_byte*)pool + sizeof(Nginx_MemoryPool);
                pool->_active = pool;
                pool = poolNext;
            }

        }

    private:

        typedef Nginx_MemoryPool    t_pool;

        typedef struct s_bigBlock
        {
            void*               _mallocedBlock;
            struct s_bigBlock*  _nextBlock;
        }   t_bigBlock;




        void*               _mallocedBlock;
        t_byte*             _freePosition;
        t_byte*             _endOfBlock;
        size_t              _blockSize;
        size_t              _capacity;    
        Nginx_MemoryPool*   _nextBlock;
        Nginx_MemoryPool*   _active;
        t_bigBlock*         _bigBlocks;

        Nginx_MemoryPool(size_t dummy) {(void)dummy;};
        
        static Nginx_MemoryPool*   _allocateNewBlock(size_t blockSize)
        {
            Nginx_MemoryPool* pool = (Nginx_MemoryPool*) new t_byte[blockSize];
            pool->_freePosition = (t_byte*)pool + sizeof(Nginx_MemoryPool);
            pool->_endOfBlock = (t_byte*)pool + blockSize;
            pool->_blockSize = blockSize;
            pool->_mallocedBlock = pool;
            pool->_capacity = blockSize - sizeof(Nginx_MemoryPool);
            pool->_bigBlocks = NULL;
            pool->_nextBlock = NULL;
            pool->_active = pool;
            return (pool);
        }

};

int main()
{
    Nginx_MemoryPool* pool = Nginx_MemoryPool::create(1024, 10);
    Nginx_MemoryPool::reset(pool);
    Nginx_MemoryPool::destroy(pool);
    return 0;
}

#endif