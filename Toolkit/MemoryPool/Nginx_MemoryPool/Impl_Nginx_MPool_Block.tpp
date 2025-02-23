

#ifndef IMPL_NGINX_MPOOL_BLOCK_TPP

# define IMPL_NGINX_MPOOL_BLOCK_TPP

# include "Nginx_MemoryPool.hpp"

template < typename T, typename Allocator >
typename Impl_Nginx_MemoryPool<T, Allocator>::Nginx_MPool_Block*
Impl_Nginx_MemoryPool<T, Allocator>::Nginx_MPool_Block::
create(size_t blockSize, size_t blockId, const TypeAllocator& allocator)
{
    TypeAllocator 		temp = allocator;
	int 				objSpace;
	T*					location;
	Nginx_MPool_Block*	pool;

	location = temp.allocate(blockSize);

	//make room for the block object itself, inside the pool
	objSpace = (sizeof(Nginx_MPool_Block) <= sizeof(T)) ? 1 : sizeof(Nginx_MPool_Block) / sizeof(T);

	pool = (Nginx_MPool_Block*) location;

    new (pool) Nginx_MPool_Block (allocator);

    pool->m_freePosition = location + objSpace;
    pool->m_endOfBlock = location + blockSize;
    pool->m_nextBlock = NULL;
    pool->m_blockId = blockId;
    pool->m_allocator = temp;

    return (pool);
}

template < typename T, typename Allocator >
void
Impl_Nginx_MemoryPool<T, Allocator>::Nginx_MPool_Block::
destroy()
{
    m_allocator.deallocate(reinterpret_cast<T*>(this), m_endOfBlock - reinterpret_cast<T*>(this));
}


template < typename T, typename Allocator >
void
Impl_Nginx_MemoryPool<T, Allocator>::Nginx_MPool_Block::
reset()
{
    m_freePosition = (T*)(this) + sizeof(Nginx_MPool_Block);
}

template < typename T, typename Allocator >
//all private, disable destructors and constructors
Impl_Nginx_MemoryPool<T, Allocator>::Nginx_MPool_Block::
Nginx_MPool_Block(const TypeAllocator& allocator) : m_allocator(allocator) {}

template < typename T, typename Allocator >
Impl_Nginx_MemoryPool<T, Allocator>::Nginx_MPool_Block::
~Nginx_MPool_Block() {}

template < typename T, typename Allocator >
Impl_Nginx_MemoryPool<T, Allocator>::Nginx_MPool_Block::
Nginx_MPool_Block(const Nginx_MPool_Block& block) 
{(void)block;}

template < typename T, typename Allocator >
typename Impl_Nginx_MemoryPool<T, Allocator>::Nginx_MPool_Block& 
Impl_Nginx_MemoryPool<T, Allocator>::Nginx_MPool_Block::operator=(const Nginx_MPool_Block& block) 
{(void)block; return (*this);}

#endif
