/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MPool_Block.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:25 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/07 19:25:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nginx_MemoryPool.hpp"


/*
                                            
    We add blocks to the top of the list, makes sense from a cache perspective (most recent block is the closest)
    and when adding new blocks, they will all come on top.
    Same for all other memory pools we have
    
    Same for single malloc blocks (big_blocks)

*/

Nginx_MemoryPool::Nginx_MPool_Block*
Nginx_MemoryPool::Nginx_MPool_Block::
create(size_t blockSize, size_t blockId)
{
    Nginx_MPool_Block* pool = (Nginx_MPool_Block*) new t_byte[blockSize];

    new (pool) Nginx_MPool_Block;
    pool->m_freePosition = (t_byte*)pool + sizeof(Nginx_MPool_Block);
    pool->m_endOfBlock = (t_byte*)pool + blockSize;
    pool->m_nextBlock = NULL;
    pool->m_blockId = blockId;
    return (pool);
}

void
Nginx_MemoryPool::Nginx_MPool_Block::
destroy()
{
    delete [] (t_byte*)(this);
}


void
Nginx_MemoryPool::Nginx_MPool_Block::
reset()
{
    m_freePosition = (t_byte*)(this) + sizeof(Nginx_MPool_Block);
}

//all private, disable destructors and constructors
Nginx_MemoryPool::Nginx_MPool_Block::
Nginx_MPool_Block() 
{}

Nginx_MemoryPool::Nginx_MPool_Block::
~Nginx_MPool_Block() {}

Nginx_MemoryPool::Nginx_MPool_Block::
Nginx_MPool_Block(const Nginx_MPool_Block& block) 
{(void)block;}

Nginx_MemoryPool::Nginx_MPool_Block& 
Nginx_MemoryPool::Nginx_MPool_Block::operator=(const Nginx_MPool_Block& block) 
{(void)block; return (*this);}
