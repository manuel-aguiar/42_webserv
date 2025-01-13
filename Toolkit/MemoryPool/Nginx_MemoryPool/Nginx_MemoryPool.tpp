/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Impl_Nginx_MemoryPool.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 22:26:55 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_MEMORYPOOL_TPP

# define NGINX_MEMORYPOOL_TPP

// C++ headers
# include <climits>
# include <cstddef>
# include <cassert>
# include <new>


template <typename T, typename Allocator>
class Impl_Nginx_MemoryPool
{
    public:

        typedef typename Allocator::template rebind<T>::other TypeAllocator;

        Impl_Nginx_MemoryPool(size_t blockSize, size_t startingBlocks = 1, const Allocator& allocator = Allocator());
        ~Impl_Nginx_MemoryPool();

		T* 						    allocate(size_t size);
        T*                          allocate(size_t size, size_t alignment);
        void                        reset(int maxBlocks = INT_MAX);
        void                        destroy();

		int							getElementSize() const;

    private:
        // forward declarations
        class                       Nginx_MPool_Block;
        typedef struct s_bigBlock
        {
            size_t              m_size;
            struct s_bigBlock*  m_nextBlock;
        }   t_bigBlock;


        //member variables
        Nginx_MPool_Block*     m_active;
        t_bigBlock*            m_bigBlocks;
        const size_t           m_blockSize;
        TypeAllocator          m_allocator;

        //block class of which the pool is made, private to pool
        class Nginx_MPool_Block
        {
            public:

                static Nginx_MPool_Block*   create(size_t blockSize, size_t blockId, const TypeAllocator& allocator = TypeAllocator());
                void                        destroy();
                void                        reset();

                T*                          m_freePosition;
                T*                          m_endOfBlock;
                Nginx_MPool_Block*          m_nextBlock;
                int                         m_blockId;
                TypeAllocator               m_allocator;
                
            private:
                Nginx_MPool_Block(const TypeAllocator& allocator = TypeAllocator());
                ~Nginx_MPool_Block();
                Nginx_MPool_Block(const Nginx_MPool_Block& block);
                Nginx_MPool_Block& operator=(const Nginx_MPool_Block& block);
        };
        
        // private copy and assignment and default construction
        Impl_Nginx_MemoryPool();
        Impl_Nginx_MemoryPool(const Impl_Nginx_MemoryPool& pool);
        Impl_Nginx_MemoryPool& operator=(const Impl_Nginx_MemoryPool& pool);

        // helper functions

        static T*           mf_allignedAlloc(T* position, size_t alignment);
        void                mf_deleteBigBlocks();
        void                mf_AddNewBlock();
        T*                  mf_AllocateBigBlock(size_t size);
        T*                  mf_AllocateSmall(size_t size, size_t alignment);             

};

# include "Impl_Nginx_MemoryPool.tpp"
# include "Impl_Nginx_MPool_Block.tpp"

#endif



