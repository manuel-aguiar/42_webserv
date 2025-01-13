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

#ifndef IMPL_NGINX_MEMORYPOOL_TPP

# define IMPL_NGINX_MEMORYPOOL_TPP

// C++ headers
# include <climits>
# include <cstddef>
# include <cassert>
# include <new>


typedef unsigned char t_byte;

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
T*   Impl_Nginx_MemoryPool<T, Allocator>::allocate(size_t size)
{
	
	return (allocate(size, (size < sizeof(size_t)) ? size : sizeof(size_t)));
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



////////////////////////////////////////////////////////


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
