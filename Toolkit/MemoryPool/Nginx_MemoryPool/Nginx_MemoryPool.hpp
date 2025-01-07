/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MemoryPool.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 19:47:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_MEMORYPOOL_HPP

# define NGINX_MEMORYPOOL_HPP

// C++ headers
# include <limits>
# include <climits>
# include <cstddef>
# include <exception>
# include <stdexcept>
# include <iostream>
# include <cassert>


typedef unsigned char t_byte;

/*

    Nginx-style non deallocating pool

*/

class Nginx_MemoryPool
{
    public:
        Nginx_MemoryPool(size_t blockSize, size_t startingBlocks = 1);
        ~Nginx_MemoryPool();

		void* 						allocate(size_t size);
        void*                       allocate(size_t size, size_t alignment);
        void                        reset(int maxBlocks = INT_MAX);
        void                        destroy();

    private:
        // forward declarations
        class                       Nginx_MPool_Block;
        typedef struct s_bigBlock
        {
            struct s_bigBlock*  m_nextBlock;
        }   t_bigBlock;


        //member variables
        Nginx_MPool_Block*     m_active;
        t_bigBlock*            m_bigBlocks;
        const size_t           m_blockSize;

        //block class of which the pool is made, private to pool
        class Nginx_MPool_Block
        {
            public:

                static Nginx_MPool_Block*   create(size_t blockSize, size_t blockId);
                void                        destroy();
                void                        reset();

                t_byte*                     m_freePosition;
                t_byte*                     m_endOfBlock;
                Nginx_MPool_Block*          m_nextBlock;
                int                         m_blockId;
                
            private:
                Nginx_MPool_Block();
                ~Nginx_MPool_Block();
                Nginx_MPool_Block(const Nginx_MPool_Block& block);
                Nginx_MPool_Block& operator=(const Nginx_MPool_Block& block);
        };
        
        // private copy and assignment and default construction
        Nginx_MemoryPool();
        Nginx_MemoryPool(const Nginx_MemoryPool& pool);
        Nginx_MemoryPool& operator=(const Nginx_MemoryPool& pool);

        // helper functions

        static t_byte*      mf_allignedAlloc(void *byte, size_t alignment);
        void                mf_deleteBigBlocks();
        void                mf_AddNewBlock();
        void*               mf_AllocateBigBlock(size_t size);
        void*               mf_AllocateSmall(size_t size, size_t alignment);             

};


#endif
