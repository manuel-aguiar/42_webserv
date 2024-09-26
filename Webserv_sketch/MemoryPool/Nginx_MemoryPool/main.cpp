/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:30:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/26 15:47:01 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*

    Fixed sized blocks. if the alloc is large, ask directly from the system using malloc
    Pool saves a pointer to the current empty block pool
    saves a list of

*/


#ifndef NGINX_MEMORYPOOL_HPP

# define NGINX_MEMORYPOOL_HPP

# include <climits>
# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <cassert>
# define BLOCK_SIZE_MAX 4096


typedef unsigned char t_byte;

/*

    Nginx-style non deallocating pool

*/

static t_byte*   allignedAlloc(void *byte, size_t alignment)
{
    //clears the bits up to natural alignment, in x86 it is size_t = 8 , so last 3 bits are 0
    return ((t_byte *) (((size_t) (byte) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
}


class Nginx_MPool_Block
{
    public:

        Nginx_MPool_Block() {throw std::logic_error("Nginx_MPool_Block, can't instantiate out of the box, has to go through static .create()");}
        ~Nginx_MPool_Block() {}
        
        /*
        
            We add blocks to the top of the list, makes sense from a cache perspective (most recent block is the closest)
            and when adding new blocks, they will all come on top.
            Same for all other memory pools we have
            
            Same for single malloc blocks (big_blocks)
        
        */

        static Nginx_MPool_Block* create(size_t blockSize, size_t startingBlocks = 1)
        {
            Nginx_MPool_Block* pool;
            Nginx_MPool_Block* next;

            next = NULL;
            for (size_t i = 0; i < startingBlocks; ++i)
            {
                pool = _allocateNewBlock(blockSize, i);
                pool->_nextBlock = next;
                next = pool;
            }
            return (pool);
        }

        static void destroy(Nginx_MPool_Block**   poolPlace)
        {
            Nginx_MPool_Block*          poolNext;
            Nginx_MPool_Block*          pool;

            pool = *poolPlace;
            while (pool)
            {
                poolNext = pool->_nextBlock;
                delete [] (t_byte*)(pool);
                pool = poolNext;
            }
            *poolPlace = NULL;
        }

        
        /*
        
            When resetting, delete the big blocks, we don't really know their size of use
            let the next pool user to take care of it
        
        */

        static void reset(Nginx_MPool_Block** pool, int maxBlocks = INT_MAX)
        {
            Nginx_MPool_Block*          poolCur;
            Nginx_MPool_Block*          poolNext;

            poolCur = (*pool);
            while (poolCur)
            {
                poolNext = poolCur->_nextBlock;
                poolCur->_freePosition = (t_byte*)pool + sizeof(Nginx_MPool_Block);
                if (poolCur->_blockId >= maxBlocks)
                    delete [] (t_byte*)(poolCur);
                else if (poolCur->_blockId == maxBlocks - 1)
                    *pool = poolCur;
                    
                poolCur = poolNext;
            }
        }

        static void*   allocate(Nginx_MPool_Block**   poolPlace, size_t size, bool aligned, size_t blockSizeAgainLol)
        {
            void*                       location;
            Nginx_MPool_Block*     pool;

            assert ((size_t)((*poolPlace)->_endOfBlock) - (size_t)((*poolPlace) - (sizeof(Nginx_MPool_Block))) >= size);
            pool = *poolPlace;

            // try to find a good allocation position, mehh, O(n)....
            while (pool) {
                t_byte* aligned_position = pool->_freePosition;
                if (aligned)
                    aligned_position = allignedAlloc(pool->_freePosition, sizeof(size_t));
                if (aligned_position + size <= pool->_endOfBlock) {
                    break; // Fits, so we can allocate here
                }
                pool = pool->_nextBlock;
            }
            if (!pool)
            {
                Nginx_MPool_Block* newPool = Nginx_MPool_Block::_allocateNewBlock(blockSizeAgainLol, (*poolPlace)->_blockId + 1);
                newPool->_nextBlock = *poolPlace;
                
                *poolPlace = newPool;
                pool = newPool;
            }
            location = pool->_freePosition;
            if (aligned)
                location = allignedAlloc(pool->_freePosition, sizeof(size_t));
            pool->_freePosition = (t_byte*)((size_t)location + size);
            return (location);
        }


        void*                       _data;
        t_byte*                     _freePosition;
        t_byte*                     _endOfBlock;
        Nginx_MPool_Block*          _nextBlock;
        int                         _blockId;

        Nginx_MPool_Block(size_t dummy) {(void)dummy;};

        static Nginx_MPool_Block*   _allocateNewBlock(size_t blockSize, int blockId)
        {
            Nginx_MPool_Block* pool = (Nginx_MPool_Block*) new t_byte[blockSize];
            pool->_freePosition = (t_byte*)pool + sizeof(Nginx_MPool_Block);
            pool->_endOfBlock = (t_byte*)pool + blockSize;
            pool->_data = pool;
            pool->_nextBlock = NULL;
            pool->_blockId = blockId;
            return (pool);
        }

};

class Nginx_MemoryPool
{
    public:
        Nginx_MemoryPool() : _active (NULL), _bigBlocks(NULL), _blockSize(0)
        {
            
        }
        ~Nginx_MemoryPool()
        {
            if (_active)
                destroy();
        }

        void    initialize(size_t blockSize, size_t startingBlocks = 1)
        {
            assert(_active == NULL);
            _active = Nginx_MPool_Block::create(blockSize, startingBlocks);
            _blockSize = blockSize;
        }

        static Nginx_MemoryPool* create(size_t blockSize, size_t startingBlocks = 1)
        {
            Nginx_MemoryPool*           newPool;
            Nginx_MPool_Block*          block;

            block = Nginx_MPool_Block::create(blockSize, startingBlocks);
            newPool = (Nginx_MemoryPool*)Nginx_MPool_Block::allocate(&block, sizeof(Nginx_MemoryPool), true, blockSize);
            new (newPool) Nginx_MemoryPool();   //placement new inside the pool itself
            newPool->_active = block;
            newPool->_blockSize = blockSize;
            return (newPool);
        }

        void   reactivate(size_t blockSize, size_t startingBlocks = 1)
        {
            assert(_active == NULL);
            _active = Nginx_MPool_Block::create(blockSize, startingBlocks);
        }

        void*   allocate(size_t size, bool aligned)
        {
            assert(_active != NULL);

            t_bigBlock*                 newBig;
            
            if (size > _blockSize  - sizeof(*_active))  //for sure doesn't fit
            {
                //big blocks don't need alignment, new already allocates according to the system alignment
                newBig = (t_bigBlock*) new t_byte[sizeof(t_bigBlock) + size];
                newBig->_data = (t_byte*)newBig + sizeof(t_bigBlock);           
                newBig->_nextBlock = _bigBlocks;
                _bigBlocks = newBig;
                return (newBig->_data);
            }
            return (Nginx_MPool_Block::allocate(&_active, size, aligned, _blockSize));
        }

        void    reset(int maxBlocks = INT_MAX)
        {
            assert(_active != NULL);
            Nginx_MPool_Block::reset(&_active, maxBlocks);
        }

        void    destroy()
        {
            t_bigBlock*                 bigCur;
            t_bigBlock*                 bigNext;

            assert(_active != NULL);
            bigCur = _bigBlocks;
            while (bigCur)
            {
                bigNext = bigCur->_nextBlock;
                delete [] (t_byte*)(bigCur);
                bigCur = bigNext;
            }
            Nginx_MPool_Block::destroy(&_active);
        }

    private:

        typedef struct s_bigBlock
        {
            void*               _data;
            struct s_bigBlock*  _nextBlock;
        }   t_bigBlock;


        Nginx_MPool_Block*     _active;
        t_bigBlock*            _bigBlocks;
        size_t                 _blockSize;

        Nginx_MemoryPool(const Nginx_MemoryPool& pool) {(void)pool;}
        Nginx_MemoryPool& operator=(const Nginx_MemoryPool& pool) {(void)pool; return (*this);}
        
};

class PoolUser
{
    public:

        static PoolUser* create(std::string name, size_t age)
        {
            PoolUser*           user;   
            Nginx_MemoryPool*   pool;
            pool = Nginx_MemoryPool::create(1024, 5);
            user = (PoolUser*)pool->allocate(sizeof(PoolUser), true);
            new (user) PoolUser(name, age);
            user->_pool = pool;
            return (user);
        }


        void destroy()
        {
            _pool->destroy();
        }


        PoolUser() : _name (""), _age(0)
        {

        }
        PoolUser(std::string name, size_t age) : _name(name), _age(age)
        {
            
        }
        std::string         _name;
        size_t              _age;
        Nginx_MemoryPool*   _pool;
};


#include <cstring>
int main()
{  
    // if you want to have a small abstraction on top
    Nginx_MemoryPool pool;
    pool.initialize(1024, 5);

    for (int i = 0; i < 100; i++)
    {
        int& aString = *(int *)pool.allocate((sizeof(std::string)), true);
        (void)aString;
        new (&aString) int(3); // placement new, construct the string on the location given
        std::cout << aString << std::endl;
    }
    pool.reset(5);
    pool.allocate(10000, true); // big block
    pool.allocate(10000, true); // big block
    pool.allocate(10000, true); // big block
    pool.allocate(10000, true); // big block
    pool.allocate(10000, true); // big block
    
    char *str = (char *)pool.allocate(sizeof("Hello World!"), true);
    std::memcpy(str, "Hello World!", sizeof("Hello World!"));
    
    
    std::cout << str << std::endl;

    pool.destroy();
    
    PoolUser* user = PoolUser::create("Miguel", 25);
    std::cout << user->_name << std::endl;

    Nginx_MemoryPool* pool2 = Nginx_MemoryPool::create(1024, 5);
    pool2->destroy();
    user->destroy();

    return 0;
}

#endif