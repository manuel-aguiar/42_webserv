/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_FixedBlock.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 22:17:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_FIXEDPOOL_HPP

# define NGINX_FIXEDPOOL_HPP

// C++ headers
# include <cstddef>
# include <cassert>
# include <iostream>

typedef unsigned char t_byte;

/*

    Nginx-style non deallocating pool

*/

template <typename Allocator>
class Heap_MemoryPool
{
    public:
        Heap_MemoryPool(size_t blockSize, const Allocator& allocator = Allocator()) :
            m_allocator(allocator),
            m_array((t_byte*)m_allocator.allocate(blockSize)), 
            m_freePosition(m_array),
            m_endOfBlock(m_array + blockSize),
            m_blockSize(blockSize) {}

        ~Heap_MemoryPool() { destroy(); }

        void* allocate(size_t size) {
            assert(m_array != NULL);
            return (allocate(size, (size > sizeof(size_t)) ? sizeof(size_t) : size));
        }

        void* allocate(size_t size, size_t alignment) {
            assert(m_array != NULL);
            t_byte* location = mf_allignedAlloc(m_freePosition, alignment);
            assert(location + size <= m_endOfBlock);
            m_freePosition = (t_byte*)((size_t)location + size);
            return (location);    
        }

        void reset() { m_freePosition = m_array; }

        void destroy()
        {
            m_allocator.deallocate(m_array, m_blockSize);
            m_array = NULL;
        }

        size_t getBlockSize() const { return (m_blockSize); }

        size_t getFreeSpace() const { return (m_endOfBlock - m_freePosition); } 

        const Allocator& getAllocator() const { return (m_allocator); }

    private:
        Allocator       m_allocator;
        t_byte*         m_array;
        t_byte*         m_freePosition;
        t_byte*         m_endOfBlock;
        const size_t    m_blockSize;

        Heap_MemoryPool() : m_array(NULL), m_blockSize(0) {}
        Heap_MemoryPool(const Heap_MemoryPool& copy) : m_blockSize(copy.m_blockSize) {(void)copy;}
        Heap_MemoryPool& operator=(const Heap_MemoryPool& assign) {(void)assign; return (*this);}

        static t_byte* mf_allignedAlloc(void *byte, size_t alignment) {
            return ((t_byte*)(((size_t)(byte) + ((size_t)alignment - 1)) & ~((size_t)alignment - 1)));
        }
};


#endif
