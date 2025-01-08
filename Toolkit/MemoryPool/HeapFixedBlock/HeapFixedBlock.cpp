/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeapFixedBlock.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 22:17:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeapFixedBlock.hpp"


HeapFixedBlock::HeapFixedBlock(size_t blockSize) :
    m_array(new t_byte[blockSize]), 
    m_freePosition(m_array),
    m_endOfBlock(m_array + blockSize),
    m_blockSize(blockSize) {}

HeapFixedBlock::~HeapFixedBlock()
{
    destroy();
}

void*   HeapFixedBlock::allocate(size_t size)
{
    assert(m_array != NULL);

	return (allocate(size, (size > sizeof(size_t)) ? sizeof(size_t) : size));
}

void*   HeapFixedBlock::allocate(size_t size, size_t alignment)
{
    assert(m_array != NULL);

    t_byte* location = mf_allignedAlloc(m_freePosition, alignment);

    assert(location + size <= m_endOfBlock);

    m_freePosition = (t_byte*)((size_t)location + size);
    return (location);    
}

size_t          HeapFixedBlock::getBlockSize() const 
{
    return (m_blockSize);
}

size_t          HeapFixedBlock::getFreeSpace() const 
{
    return (m_endOfBlock - m_freePosition);
} 

void    HeapFixedBlock::reset()
{
    m_freePosition = m_array;
}

void    HeapFixedBlock::destroy()
{
    delete [] (m_array);
    m_array = NULL;
}

t_byte*   HeapFixedBlock::mf_allignedAlloc(void *byte, size_t alignment)
{
    //clears the bits up to natural alignment, in x86 it is size_t = 8 , so last 3 bits are 0
    return ((t_byte *) (((size_t) (byte) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
}

//private, no copies
HeapFixedBlock::HeapFixedBlock(const HeapFixedBlock& copy) : m_blockSize(copy.m_blockSize) {(void)copy;}
HeapFixedBlock& HeapFixedBlock::operator=(const HeapFixedBlock& assign) {(void)assign; return (*this);}
HeapFixedBlock::HeapFixedBlock() : m_array(NULL), m_blockSize(0) {}

