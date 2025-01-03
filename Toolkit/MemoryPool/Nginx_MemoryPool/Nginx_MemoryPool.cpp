/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MemoryPool.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 16:21:20 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nginx_MemoryPool.hpp"


Nginx_MemoryPool::Nginx_MemoryPool(size_t blockSize, size_t startingBlocks) :
    m_active(NULL), m_bigBlocks(NULL), m_blockSize(blockSize)
{
    Nginx_MPool_Block*          curBlock;
    Nginx_MPool_Block*          prevBlock;

    curBlock = Nginx_MPool_Block::create(blockSize, 1);
    for (size_t i = 1; i < startingBlocks; ++i)
    {
        prevBlock = Nginx_MPool_Block::allocateNewBlock(blockSize, i);
        prevBlock->m_nextBlock = curBlock;
        curBlock = prevBlock;
    }
    m_active = curBlock;
}

Nginx_MemoryPool::~Nginx_MemoryPool()
{
    destroy();
}

void*   Nginx_MemoryPool::allocate(size_t size)
{
	return (allocate(size, std::min(size, sizeof(size_t))));
}

void*   Nginx_MemoryPool::allocate(size_t size, size_t alignment)
{
    t_bigBlock*                 newBig;

    if (size > m_blockSize  - sizeof(Nginx_MPool_Block))
    {
        newBig = (t_bigBlock*) new t_byte[sizeof(t_bigBlock) + size];
        newBig->m_data = (t_byte*)newBig + sizeof(t_bigBlock);
        newBig->m_nextBlock = m_bigBlocks;
        m_bigBlocks = newBig;
        return (newBig->m_data);
    }
    
    if (!m_active)
        m_active = Nginx_MPool_Block::create(m_blockSize, 1);

    return (Nginx_MPool_Block::allocate(&m_active, size, alignment, m_blockSize));
}

void    Nginx_MemoryPool::reset(int maxBlocks)
{
    mf_deleteBigBlocks();
    
    if (m_active)
        Nginx_MPool_Block::reset(&m_active, maxBlocks);
}

void    Nginx_MemoryPool::destroy()
{
    mf_deleteBigBlocks();
    
    Nginx_MPool_Block::destroy(&m_active);
    m_active = NULL;
}

void    Nginx_MemoryPool::mf_deleteBigBlocks()
{
    t_bigBlock*                 bigCur;
    t_bigBlock*                 bigNext;

    bigCur = m_bigBlocks;
    while (bigCur)
    {
        bigNext = bigCur->m_nextBlock;
        delete [] (t_byte*)(bigCur);
        bigCur = bigNext;
    }
    m_bigBlocks = NULL;
}

t_byte*   Nginx_MemoryPool::mf_allignedAlloc(void *byte, size_t alignment)
{
    //clears the bits up to natural alignment, in x86 it is size_t = 8 , so last 3 bits are 0
    return ((t_byte *) (((size_t) (byte) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
}

//private, no copies



Nginx_MemoryPool::Nginx_MemoryPool(const Nginx_MemoryPool& copy) : m_blockSize(copy.m_blockSize) {(void)copy;}
Nginx_MemoryPool& Nginx_MemoryPool::operator=(const Nginx_MemoryPool& assign) {(void)assign; return (*this);}
Nginx_MemoryPool::Nginx_MemoryPool() : m_active(NULL), m_bigBlocks(NULL), m_blockSize(0) {}



// helper function, looks weird here

