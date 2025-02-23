

#ifndef IMPL_NGINX_MEMORYPOOL_TPP

# define IMPL_NGINX_MEMORYPOOL_TPP

# include "Nginx_MemoryPool.hpp"

template < typename T, typename Allocator >
Impl_Nginx_MemoryPool<T, Allocator>::Impl_Nginx_MemoryPool(size_t blockSize, size_t startingBlocks, const Allocator& allocator) :
	m_active(NULL), 
	m_bigBlocks(NULL), 
	m_blockSize(blockSize),
    m_allocator(allocator)
{
	for (size_t i = 0; i < startingBlocks; ++i)
		mf_AddNewBlock();
}

template < typename T, typename Allocator >
void    Impl_Nginx_MemoryPool<T, Allocator>::mf_AddNewBlock()
{
	Nginx_MPool_Block*	newBlock;
	int					id;

	id = (m_active) ? m_active->m_blockId + 1 : 1;
	
	newBlock = Nginx_MPool_Block::create(m_blockSize, id, m_allocator);
	newBlock->m_nextBlock = m_active;
	m_active = newBlock;
}

template < typename T, typename Allocator >
Impl_Nginx_MemoryPool<T, Allocator>::~Impl_Nginx_MemoryPool()
{
	destroy();
}

template < typename T, typename Allocator >
int   Impl_Nginx_MemoryPool<T, Allocator>::getElementSize() const
{
	return (sizeof(T));
}

template < typename T, typename Allocator >
T*   Impl_Nginx_MemoryPool<T, Allocator>::allocate(size_t size)
{
	
	return (allocate(size, (size * sizeof(T) < sizeof(size_t)) ? size * sizeof(T) : sizeof(size_t)));
}

template < typename T, typename Allocator >
T*	Impl_Nginx_MemoryPool<T, Allocator>::mf_AllocateBigBlock(size_t size)
{
	T*						  	location;
	t_bigBlock*                 newBig;
	int						 	objSpace;

	// make room for the struct in the new block
	objSpace = (sizeof(t_bigBlock) <= sizeof(T)) ? 1 : sizeof(t_bigBlock) / sizeof(T);

	location = m_allocator.allocate(objSpace + size);

	newBig = (t_bigBlock*) location;
	newBig->m_nextBlock = m_bigBlocks;
    newBig->m_size = objSpace + size;
	m_bigBlocks = newBig;

	return (location + objSpace);
}

template < typename T, typename Allocator >
T*	Impl_Nginx_MemoryPool<T, Allocator>::mf_AllocateSmall(size_t size, size_t alignment)
{
	T*                          location;
	Nginx_MPool_Block*          block;

	if (!m_active)
		m_active = Nginx_MPool_Block::create(m_blockSize, 1, m_allocator);

	block = m_active;

	while (block)
	{
		T* aligned_position = block->m_freePosition;
		aligned_position = mf_allignedAlloc(block->m_freePosition, alignment);
		if (aligned_position + size <= block->m_endOfBlock)
			break;
		block = block->m_nextBlock;
	}
	if (!block)
	{
		Nginx_MPool_Block* newPool = Nginx_MPool_Block::create(m_blockSize, m_active->m_blockId + 1, m_allocator);
		newPool->m_nextBlock = m_active;
		
		m_active = newPool;
		block = newPool;
	}
	location = block->m_freePosition;
	location = mf_allignedAlloc(block->m_freePosition, alignment);
	block->m_freePosition = location + size;
	return (location);   	
}

template < typename T, typename Allocator >
T*   Impl_Nginx_MemoryPool<T, Allocator>::allocate(size_t size, size_t alignment)
{
	if (size > m_blockSize  - sizeof(Nginx_MPool_Block))
		return (mf_AllocateBigBlock(size));
	return (mf_AllocateSmall(size, alignment));
}

template < typename T, typename Allocator >
void    Impl_Nginx_MemoryPool<T, Allocator>::reset(int maxBlocks)
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
			poolCur->destroy();
		else if (poolCur->m_blockId == maxBlocks - 1)
			m_active = poolCur;
		poolCur = poolNext;
	}
}

template < typename T, typename Allocator >
void    Impl_Nginx_MemoryPool<T, Allocator>::destroy()
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

template < typename T, typename Allocator >
void    Impl_Nginx_MemoryPool<T, Allocator>::mf_deleteBigBlocks()
{
	t_bigBlock*                 bigCur;
	t_bigBlock*                 bigNext;

	bigCur = m_bigBlocks;
	while (bigCur)
	{
		bigNext = bigCur->m_nextBlock;
		m_allocator.deallocate((T*)bigCur, bigCur->m_size);
		bigCur = bigNext;
	}
	m_bigBlocks = NULL;
}

template < typename T, typename Allocator >
T*   Impl_Nginx_MemoryPool<T, Allocator>::mf_allignedAlloc(T* position, size_t alignment)
{
	//clears the bits up to natural alignment, in x86 it is size_t = 8 , so last 3 bits are 0
	return ((T *) (((size_t) (position) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
}

//private, no copies

template < typename T, typename Allocator >
Impl_Nginx_MemoryPool<T, Allocator>::Impl_Nginx_MemoryPool(const Impl_Nginx_MemoryPool& copy) : m_blockSize(copy.m_blockSize) {(void)copy;}

template < typename T, typename Allocator >
Impl_Nginx_MemoryPool<T, Allocator>& Impl_Nginx_MemoryPool<T, Allocator>::operator=(const Impl_Nginx_MemoryPool& assign) {(void)assign; return (*this);}

template < typename T, typename Allocator >
Impl_Nginx_MemoryPool<T, Allocator>::Impl_Nginx_MemoryPool() : m_active(NULL), m_bigBlocks(NULL), m_blockSize(0) {}

#endif
