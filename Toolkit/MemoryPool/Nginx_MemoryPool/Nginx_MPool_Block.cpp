/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MPool_Block.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 18:25:01 by mmaria-d         ###   ########.fr       */
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
    Nginx_MPool_Block* pool = NULL;
    Nginx_MPool_Block* m_next;

    m_next = NULL;
    for (size_t i = 0; i < startingBlocks; ++i)
    {
        pool = allocateNewBlock(blockSize, i);
        pool->m_nextBlock = m_next;
        m_next = pool;
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
        poolNext = pool->m_nextBlock;
        delete [] (t_byte*)(pool);
        pool = poolNext;
    }
}

/*

    When resetting, delete the big blocks, we don't really know their size of use
    let the m_next pool user to take care of it

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
        poolNext = poolCur->m_nextBlock;
        poolCur->m_freePosition = (t_byte*)poolCur + sizeof(Nginx_MPool_Block);
        if (poolCur->m_blockId == 0)
            poolCur->m_freePosition += sizeof(Nginx_MemoryPool);
        if (poolCur->m_blockId >= maxBlocks)
            delete [] (t_byte*)(poolCur);
        else if (poolCur->m_blockId == maxBlocks - 1)
            *pool = poolCur;
        poolCur = poolNext;
    }
}


void *
Nginx_MemoryPool::Nginx_MPool_Block::
allocate(Nginx_MPool_Block**   poolPlace, size_t size, size_t alignment, size_t blockSizeAgainLol)
{
    void*                       location;
    Nginx_MPool_Block*     pool;

    assert ((size_t)((*poolPlace)->m_endOfBlock) - (size_t)((*poolPlace) - (sizeof(Nginx_MPool_Block))) >= size);
    pool = *poolPlace;

    // try to find a good allocation position, mehh, O(n)....
    while (pool)
    {
        t_byte* aligned_position = pool->m_freePosition;
        aligned_position = allignedAlloc(pool->m_freePosition, alignment);
        if (aligned_position + size <= pool->m_endOfBlock)
            break;
        pool = pool->m_nextBlock;
    }
    if (!pool)
    {
        Nginx_MPool_Block* newPool = Nginx_MPool_Block::allocateNewBlock(blockSizeAgainLol, (*poolPlace)->m_blockId + 1);
        newPool->m_nextBlock = *poolPlace;
        
        *poolPlace = newPool;
        pool = newPool;
    }
    location = pool->m_freePosition;
    location = allignedAlloc(pool->m_freePosition, alignment);
    pool->m_freePosition = (t_byte*)((size_t)location + size);
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
allocateNewBlock(size_t blockSize, int blockId)
{
    Nginx_MPool_Block* pool = (Nginx_MPool_Block*) new t_byte[blockSize];
    new (pool) Nginx_MPool_Block(0);
    pool->m_freePosition = (t_byte*)pool + sizeof(Nginx_MPool_Block);
    pool->m_endOfBlock = (t_byte*)pool + blockSize;
    pool->m_data = pool;
    pool->m_nextBlock = NULL;
    pool->m_blockId = blockId;
    return (pool);
}