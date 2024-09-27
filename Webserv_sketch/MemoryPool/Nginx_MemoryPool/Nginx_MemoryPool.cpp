/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MemoryPool.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 11:40:26 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nginx_MemoryPool.hpp"

Nginx_MemoryPool::Nginx_MemoryPool()
{throw std::logic_error("Nginx_MemoryPool, can't instantiate out of the box, has to go through static .create()");}

//since pool is self hosted on its first memoryblock, it is up to the user to call .destroy()    
Nginx_MemoryPool::~Nginx_MemoryPool() {}

Nginx_MemoryPool* Nginx_MemoryPool::create(size_t blockSize, size_t startingBlocks = 1)
{
    Nginx_MemoryPool*           newPool;
    Nginx_MPool_Block*          block;

    block = Nginx_MPool_Block::create(blockSize, startingBlocks);
    newPool = (Nginx_MemoryPool*)Nginx_MPool_Block::allocate(&block, sizeof(Nginx_MemoryPool), true, blockSize);
    new (newPool) Nginx_MemoryPool(blockSize, startingBlocks);   //placement new inside the pool itself
    newPool->_active = block;
    newPool->_blockSize = blockSize;
    return (newPool);
}

void*   Nginx_MemoryPool::allocate(size_t size, bool aligned)
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

void    Nginx_MemoryPool::reset(int maxBlocks = INT_MAX)
{
    assert(_active != NULL);
    Nginx_MPool_Block::reset(&_active, maxBlocks);
}

void    Nginx_MemoryPool::destroy()
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

//private, no copies

Nginx_MemoryPool::Nginx_MemoryPool(size_t blockSize, size_t startingBlocks = 1) : 
    _active(NULL), _bigBlocks(NULL), _blockSize(blockSize)
{
    (void)startingBlocks;
}

Nginx_MemoryPool::Nginx_MemoryPool(const Nginx_MemoryPool& pool) {(void)pool;}
Nginx_MemoryPool& Nginx_MemoryPool::operator=(const Nginx_MemoryPool& pool) {(void)pool; return (*this);}


t_byte*   allignedAlloc(void *byte, size_t alignment)
{
    //clears the bits up to natural alignment, in x86 it is size_t = 8 , so last 3 bits are 0
    return ((t_byte *) (((size_t) (byte) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
}