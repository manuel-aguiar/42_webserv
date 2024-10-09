/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MemoryPool.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:01:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_MEMORYPOOL_HPP

# define NGINX_MEMORYPOOL_HPP


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
        static Nginx_MemoryPool*    create(size_t blockSize, size_t startingBlocks = 1);
        void*                       allocate(size_t size, size_t alignment = (sizeof(size_t)));
        void                        reset(int maxBlocks = INT_MAX);
        void                        destroy();

    private:

        class                       Nginx_MPool_Block;


        typedef struct s_bigBlock
        {
            void*               m_data;
            struct s_bigBlock*  m_nextBlock;
        }   t_bigBlock;

        Nginx_MPool_Block*     m_active;
        t_bigBlock*            m_bigBlocks;
        size_t                 m_blockSize;

        // private everything 
        Nginx_MemoryPool();
        ~Nginx_MemoryPool();
        Nginx_MemoryPool(size_t blockSize, size_t startingBlocks = 1);
        Nginx_MemoryPool(const Nginx_MemoryPool& pool);
        Nginx_MemoryPool& operator=(const Nginx_MemoryPool& pool);
        


        //block class of which the pool is made, private to pool
        class Nginx_MPool_Block
        {
            public:
                Nginx_MPool_Block();
                ~Nginx_MPool_Block();
                
                static Nginx_MPool_Block*   create(size_t blockSize, size_t startingBlocks = 1);
                static void                 destroy(Nginx_MPool_Block**   poolPlace);
                static void                 reset(Nginx_MPool_Block** pool, int maxBlocks = INT_MAX);
                static void*                allocate(Nginx_MPool_Block**   poolPlace, size_t size, size_t alignment, size_t blockSizeAgainLol);
                static Nginx_MPool_Block*   allocateNewBlock(size_t blockSize, int blockId);  

                //leting variables public, whole class is private to memorypool anyways
                void*                       m_data;
                t_byte*                     _freePosition;
                t_byte*                     _endOfBlock;
                Nginx_MPool_Block*          m_nextBlock;
                int                         _blockId;


            private:
                Nginx_MPool_Block(size_t dummy);
                Nginx_MPool_Block(const Nginx_MPool_Block& block);
                Nginx_MPool_Block& operator=(const Nginx_MPool_Block& block);
        };
        
};

t_byte*   allignedAlloc(void *byte, size_t alignment);

#endif