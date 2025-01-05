/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NginxMemPool_TestCounter.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:29:13 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/03 16:47:36 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "NginxMemPool_TestCounter.hpp"

NginxMemPool_TestCounter::NginxMemPool_TestCounter(size_t blockSize, size_t startingBlocks, AllocationStats* placeStats) : 
    Nginx_MemoryPool(blockSize, startingBlocks),
    m_stats(placeStats) {}

NginxMemPool_TestCounter::~NginxMemPool_TestCounter() {}

void* NginxMemPool_TestCounter::allocate(size_t size)
{
    void* ptr = Nginx_MemoryPool::allocate(size);

    (m_stats->m_allocCount)++; 
    (m_stats->m_allocBytes) += size;
    return (ptr);
}

void* NginxMemPool_TestCounter::allocate(size_t size, size_t alignment)
{
    void* ptr = Nginx_MemoryPool::allocate(size, alignment);
    
    (m_stats->m_allocCount)++; 
    (m_stats->m_allocBytes) += size;
    return ptr;
}

void NginxMemPool_TestCounter::reset(int maxBlocks)
{
    Nginx_MemoryPool::reset(maxBlocks);
}

void NginxMemPool_TestCounter::destroy()
{
    Nginx_MemoryPool::destroy();
}