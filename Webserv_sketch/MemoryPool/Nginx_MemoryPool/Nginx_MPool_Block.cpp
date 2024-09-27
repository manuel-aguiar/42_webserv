/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MPool_Block.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 12:05:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nginx_MemoryPool.hpp"

Nginx_MemoryPool::Nginx_MPool_Block::
Nginx_MPool_Block() 
{throw std::logic_error("Nginx_MPool_Block, can't instantiate out of the box, has to go through static .create()");}


Nginx_MemoryPool::Nginx_MPool_Block::
~Nginx_MPool_Block() {}

/*
                                            
    We add blocks to the top of the list, makes sense from a cache perspective (most recent block is the closest)
    and when adding new blocks, they will all come on top.
    Same for all other memory pools we have
    
    Same for single malloc blocks (big_blocks)

*/

Nginx_MemoryPool::Nginx_MPool_Block*
Nginx_MemoryPool::Nginx_MPool_Block::
create(size_t blockSize, size_t startingBlocks)
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

void
Nginx_MemoryPool::Nginx_MPool_Block::
destroy(Nginx_MPool_Block**   poolPlace)
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
}

/*

    When resetting, delete the big blocks, we don't really know their size of use
    let the next pool user to take care of it

*/
void
Nginx_MemoryPool::Nginx_MPool_Block::
reset(Nginx_MPool_Block** pool, int maxBlocks)
{
    Nginx_MPool_Block*          poolCur;
    Nginx_MPool_Block*          poolNext;

    assert(maxBlocks > 0);  //at least one block for the pool selfhosting, otherwise UB
    poolCur = (*pool);
    while (poolCur)
    {
        poolNext = poolCur->_nextBlock;
        poolCur->_freePosition = (t_byte*)poolCur + sizeof(Nginx_MPool_Block);
        if (poolCur->_blockId >= maxBlocks)
        {
            std::cout << "deleting block " << poolCur->_blockId << std::endl;   
            delete [] (t_byte*)(poolCur);
        }
            
        else if (poolCur->_blockId == maxBlocks - 1)
        {
            std::cout << "last block " << poolCur->_blockId << std::endl;
            *pool = poolCur;
        }
            
            
        poolCur = poolNext;
    }
}


void *
Nginx_MemoryPool::Nginx_MPool_Block::
allocate(Nginx_MPool_Block**   poolPlace, size_t size, bool aligned, size_t blockSizeAgainLol)
{
    void*                       location;
    Nginx_MPool_Block*     pool;

    assert ((size_t)((*poolPlace)->_endOfBlock) - (size_t)((*poolPlace) - (sizeof(Nginx_MPool_Block))) >= size);
    pool = *poolPlace;

    // try to find a good allocation position, mehh, O(n)....
    while (pool)
    {
        t_byte* aligned_position = pool->_freePosition;
        if (aligned)
            aligned_position = allignedAlloc(pool->_freePosition, sizeof(size_t));
        if (aligned_position + size <= pool->_endOfBlock)
            break;
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


//private: no copies

// dummy for internal use
Nginx_MemoryPool::Nginx_MPool_Block::
Nginx_MPool_Block(size_t dummy) 
{(void)dummy;};

Nginx_MemoryPool::Nginx_MPool_Block::
Nginx_MPool_Block(const Nginx_MPool_Block& block) 
{(void)block;}

Nginx_MemoryPool::Nginx_MPool_Block& 
Nginx_MemoryPool::Nginx_MPool_Block::operator=(const Nginx_MPool_Block& block) 
{(void)block; return (*this);}


 /*
    Block constructs its own data inside the new'ed block after allocating
    placement new of itself inside the new'ed block, it is not needed but "explains" the concept
*/

Nginx_MemoryPool::Nginx_MPool_Block*
Nginx_MemoryPool::Nginx_MPool_Block::
_allocateNewBlock(size_t blockSize, int blockId)
{
    Nginx_MPool_Block* pool = (Nginx_MPool_Block*) new t_byte[blockSize];
    new (pool) Nginx_MPool_Block(0);
    pool->_freePosition = (t_byte*)pool + sizeof(Nginx_MPool_Block);
    pool->_endOfBlock = (t_byte*)pool + blockSize;
    pool->_data = pool;
    pool->_nextBlock = NULL;
    pool->_blockId = blockId;
    return (pool);
}