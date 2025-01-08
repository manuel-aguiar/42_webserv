/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MemoryPool.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 00:28:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nginx_MemoryPool.hpp"


Nginx_MemoryPool::Nginx_MemoryPool(size_t blockSize, size_t startingBlocks) :
	m_active(NULL), 
	m_bigBlocks(NULL), 
	m_blockSize(blockSize)
{
	for (size_t i = 0; i < startingBlocks; ++i)
		mf_AddNewBlock();
}

void    Nginx_MemoryPool::mf_AddNewBlock()
{
	Nginx_MPool_Block*	newBlock;
	int					id;

	id = (m_active) ? m_active->m_blockId + 1 : 1;
	
	newBlock = Nginx_MPool_Block::create(m_blockSize, id);
	newBlock->m_nextBlock = m_active;
	m_active = newBlock;
}

Nginx_MemoryPool::~Nginx_MemoryPool()
{
	destroy();
}

void*   Nginx_MemoryPool::allocate(size_t size)
{
	return (allocate(size, (size < sizeof(size_t)) ? size : sizeof(size_t)));
}

void*	Nginx_MemoryPool::mf_AllocateBigBlock(size_t size)
{
	t_bigBlock*                 newBig;

	newBig = (t_bigBlock*) new t_byte[sizeof(t_bigBlock) + size];
	newBig->m_nextBlock = m_bigBlocks;
	m_bigBlocks = newBig;

	return ((t_byte*)newBig + sizeof(t_bigBlock));
}

void*	Nginx_MemoryPool::mf_AllocateSmall(size_t size, size_t alignment)
{
	void*                       location;
	Nginx_MPool_Block*          block;

	if (!m_active)
		m_active = Nginx_MPool_Block::create(m_blockSize, 1);

	block = m_active;

	while (block)
	{
		t_byte* aligned_position = block->m_freePosition;
		aligned_position = mf_allignedAlloc(block->m_freePosition, alignment);
		if (aligned_position + size <= block->m_endOfBlock)
			break;
		block = block->m_nextBlock;
	}
	if (!block)
	{
		Nginx_MPool_Block* newPool = Nginx_MPool_Block::create(m_blockSize, m_active->m_blockId + 1);
		newPool->m_nextBlock = m_active;
		
		m_active = newPool;
		block = newPool;
	}
	location = block->m_freePosition;
	location = mf_allignedAlloc(block->m_freePosition, alignment);
	block->m_freePosition = (t_byte*)((size_t)location + size);
	return (location);   	
}

void*   Nginx_MemoryPool::allocate(size_t size, size_t alignment)
{
	if (size > m_blockSize  - sizeof(Nginx_MPool_Block))
		return (mf_AllocateBigBlock(size));
	return (mf_AllocateSmall(size, alignment));
}

void    Nginx_MemoryPool::reset(int maxBlocks)
{
	mf_deleteBigBlocks();
	
	if (!m_active)
		return ;

	Nginx_MPool_Block*          poolCur;
	Nginx_MPool_Block*          poolNext;

	poolCur = m_active;
	while (poolCur)
	{
		poolNext = poolCur->m_nextBlock;
		poolCur->reset();
		if (poolCur->m_blockId >= maxBlocks)
			delete [] (t_byte*)(poolCur);
		else if (poolCur->m_blockId == maxBlocks - 1)
			m_active = poolCur;
		poolCur = poolNext;
	}
}

void    Nginx_MemoryPool::destroy()
{
	mf_deleteBigBlocks();
	
	Nginx_MPool_Block*          blockNext;
	Nginx_MPool_Block*          block;

	block = m_active;
	while (block)
	{
		blockNext = block->m_nextBlock;
		block->destroy();
		block = blockNext;
	}
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
