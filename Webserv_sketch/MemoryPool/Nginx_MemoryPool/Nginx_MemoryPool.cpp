/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MemoryPool.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:01:57 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nginx_MemoryPool.hpp"

Nginx_MemoryPool::Nginx_MemoryPool() : m_active(NULL), m_bigBlocks(NULL), m_blockSize(0)
{throw std::logic_error("Nginx_MemoryPool, can't instantiate out of the box, has to go through static .create()");}

//since pool is self hosted on its first memoryblock, it is up to the user to call .destroy()    
Nginx_MemoryPool::~Nginx_MemoryPool() {}

Nginx_MemoryPool* Nginx_MemoryPool::create(size_t blockSize, size_t startingBlocks)
{
    Nginx_MemoryPool*           newPool;
    Nginx_MPool_Block*          curBlock;
    Nginx_MPool_Block*          prevBlock;

    curBlock = Nginx_MPool_Block::create(blockSize, 1);
    //std::cout << " pool created at block " << curBlock << std::endl;
    newPool = (Nginx_MemoryPool*)Nginx_MPool_Block::allocate(&curBlock, sizeof(Nginx_MemoryPool), true, blockSize);
    new (newPool) Nginx_MemoryPool(blockSize, startingBlocks);   //placement new inside block[0] of the pool itself

    for (size_t i = 1; i < startingBlocks; ++i)
    {
        prevBlock = Nginx_MPool_Block::allocateNewBlock(blockSize, i);
        prevBlock->m_nextBlock = curBlock;
        curBlock = prevBlock;
    }
    
    newPool->m_active = curBlock;
    newPool->m_blockSize = blockSize;
    return (newPool);
}

void*   Nginx_MemoryPool::allocate(size_t size, size_t alignment)
{
    assert(m_active != NULL);

    t_bigBlock*                 newBig;
    
    if (size > m_blockSize  - sizeof(*m_active))  //for sure doesn't fit
    {
        //std::cout << "big alloc " << std::endl;
        //big blocks don't need alignment, new already allocates according to the system alignment
        newBig = (t_bigBlock*) new t_byte[sizeof(t_bigBlock) + size];
        newBig->m_data = (t_byte*)newBig + sizeof(t_bigBlock);           
        newBig->m_nextBlock = m_bigBlocks;
        m_bigBlocks = newBig;
        return (newBig->m_data);
    }
    return (Nginx_MPool_Block::allocate(&m_active, size, alignment, m_blockSize));
}

void    Nginx_MemoryPool::reset(int maxBlocks)
{
    assert(m_active != NULL);
    t_bigBlock*                 bigCur;
    t_bigBlock*                 bigNext;

    bigCur = m_bigBlocks;
    //std::cout << &m_bigBlocks << std::endl;
    //std::cout << "blocksize " << m_blockSize << std::endl;
    while (bigCur)
    {
        bigNext = bigCur->m_nextBlock;
        delete [] (t_byte*)(bigCur);
        bigCur = bigNext;
    }
    //std::cout << "m_prev active: " << m_active << std::endl;
    Nginx_MPool_Block::reset(&m_active, maxBlocks);
    //std::cout << "new active: " << m_active << std::endl;
    //std::cout << m_bigBlocks << std::endl;
}

void    Nginx_MemoryPool::destroy()
{
    t_bigBlock*                 bigCur;
    t_bigBlock*                 bigNext;

    assert(m_active != NULL);
    bigCur = m_bigBlocks;
    //std::cout << &m_bigBlocks << std::endl;
    //std::cout << "blocksize " << m_blockSize << std::endl;
    while (bigCur)
    {
        bigNext = bigCur->m_nextBlock;
        delete [] (t_byte*)(bigCur);
        bigCur = bigNext;
    }
    Nginx_MPool_Block::destroy(&m_active);
    this->~Nginx_MemoryPool();
}

//private, no copies

Nginx_MemoryPool::Nginx_MemoryPool(size_t blockSize, size_t startingBlocks) : 
    m_active(NULL), m_bigBlocks(NULL), m_blockSize(blockSize)
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