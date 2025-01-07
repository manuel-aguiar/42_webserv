/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_FixedPool.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 16:40:43 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nginx_FixedPool.hpp"


Nginx_FixedPool::Nginx_FixedPool(size_t blockSize) :
    m_array(new t_byte[blockSize]), 
    m_freePosition(m_array),
    m_endOfBlock(m_array + blockSize),
    m_blockSize(blockSize) {}

Nginx_FixedPool::~Nginx_FixedPool()
{
    delete [] (m_array);
}

void*   Nginx_FixedPool::allocate(size_t size)
{
	return (allocate(size, std::min(size, sizeof(size_t))));
}

void*   Nginx_FixedPool::allocate(size_t size, size_t alignment)
{
    t_byte* location = mf_allignedAlloc(m_freePosition, alignment);

    if (location + size > m_endOfBlock)
        return (NULL);
    m_freePosition = (t_byte*)((size_t)location + size);
    return (location);    
}

size_t          Nginx_FixedPool::getBlockSize() const 
{
    return (m_blockSize);
}

size_t          Nginx_FixedPool::getFreeSpace() const 
{
    return (m_endOfBlock - m_freePosition);
} 

void    Nginx_FixedPool::reset()
{
    m_freePosition = m_array;
}

t_byte*   Nginx_FixedPool::mf_allignedAlloc(void *byte, size_t alignment)
{
    //clears the bits up to natural alignment, in x86 it is size_t = 8 , so last 3 bits are 0
    return ((t_byte *) (((size_t) (byte) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
}

//private, no copies
Nginx_FixedPool::Nginx_FixedPool(const Nginx_FixedPool& copy) : m_blockSize(copy.m_blockSize) {(void)copy;}
Nginx_FixedPool& Nginx_FixedPool::operator=(const Nginx_FixedPool& assign) {(void)assign; return (*this);}
Nginx_FixedPool::Nginx_FixedPool() : m_array(NULL), m_blockSize(0) {}

