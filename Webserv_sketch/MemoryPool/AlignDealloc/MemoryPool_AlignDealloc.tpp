/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MemoryPool_AlignDealloc.tpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:47:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 08:56:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <limits>
#include <stddef.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <stdint.h>

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
class MemoryPool_AlignDealloc
{
	public:
	
		typedef T               value_type;
		typedef T*              pointer;
		typedef T&              reference;
		typedef const T*        const_pointer;
		typedef const T&        const_reference;
		typedef size_t          size_type;
		typedef ptrdiff_t       difference_type;

		template <typename U> struct rebind {
			typedef MemoryPool_AlignDealloc<U> other;
		};

		MemoryPool_AlignDealloc() throw();
		MemoryPool_AlignDealloc(size_t block_size, size_t starting_blocks, size_t spare_blocks) throw();
		template <class U> MemoryPool_AlignDealloc(const MemoryPool_AlignDealloc<U>& memoryPool) throw();
		~MemoryPool_AlignDealloc() throw();

		pointer address(reference x) const throw();
		const_pointer address(const_reference x) const throw();

		pointer allocate(size_type n = 1, const_pointer hint = 0);
		void deallocate(pointer p, size_type n = 1);

		size_type max_size() const throw();

		void construct(pointer p, const_reference val);
		void destroy(pointer p);

		pointer newElement(const_reference val);
		void deleteElement(pointer p);

	//private:

		struct s_BlockData;


		union  s_Slot{
			char			m_data[sizeof(value_type)];
			s_Slot*			m_next;
		};

		
		typedef char* t_data_pointer;
		typedef s_Slot t_slot_type;
		typedef s_Slot* t_slot_pointer;

		struct s_BlockData {
			s_BlockData*					m_next;
			s_BlockData*					m_prev;
			size_t 						m_usedSlotsCount;
			size_t 						m_slotsCapacity;
			t_slot_pointer				m_currentSlot;
			t_slot_pointer				m_freeSlots;
		};

		size_t							m_blockSize;
		s_BlockData*						m_availableBlocks;
		s_BlockData*						m_fullBlocks;
		size_t							m_freeBlocksCount;
		size_t							m_maxFreeBlocks;	
	

		size_type padPointer(t_data_pointer p, size_type align) const throw();
		void allocateBlock();
		void deallocateBlock(s_BlockData* block); // Deallocate a fully free block
		void moveToAnotherTop(s_BlockData**	to,s_BlockData**	from, s_BlockData*	node);
		void removeBlockFromList(s_BlockData **list, s_BlockData* node);

		MemoryPool_AlignDealloc(const MemoryPool_AlignDealloc& memoryPool) throw();
		MemoryPool_AlignDealloc& operator=(const MemoryPool_AlignDealloc& memoryPool);
};



template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline typename MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::size_type
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::padPointer(t_data_pointer p, size_type align)
const throw()
{
	size_t result = reinterpret_cast<size_t>(p);
	return ((align - result) % align);
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::MemoryPool_AlignDealloc() throw() : 
	m_blockSize(BlockSize),
	m_availableBlocks(0), 
	m_fullBlocks(0),
	m_freeBlocksCount(0), 
	m_maxFreeBlocks(SpareBlocks)  // Allow up to 5 free blocks
{
	assert(((BlockSize & (BlockSize - 1)) == 0) && 
			BlockSize <= std::numeric_limits<uint16_t>::max() &&
			BlockSize >= 512); // Power of 2 check +
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::MemoryPool_AlignDealloc(size_t block_size, size_t starting_blocks, size_t spare_blocks) throw() :
	m_blockSize(block_size),
	m_availableBlocks(0), 
	m_fullBlocks(0),
	m_freeBlocksCount(0), 
	m_maxFreeBlocks(spare_blocks)  // Allow up to 5 free blocks
{
	assert(((this->m_blockSize & (this->m_blockSize - 1)) == 0) && 
			this->m_blockSize <= std::numeric_limits<uint16_t>::max() &&
			this->m_blockSize >= 512 // Power of 2 check + page limits
			&& starting_blocks >= 0 && m_maxFreeBlocks >= starting_blocks);
	
	for (size_t i = 0; i < starting_blocks; i++)
		allocateBlock();
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
template<class U>
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::MemoryPool_AlignDealloc(const MemoryPool_AlignDealloc<U>& memoryPool)
throw() :
	m_blockSize(memoryPool.m_blockSize),
	m_availableBlocks(0), 
	m_fullBlocks(0),
	m_freeBlocksCount(0), 
	m_maxFreeBlocks(memoryPool.m_maxFreeBlocks)
{
	
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::~MemoryPool_AlignDealloc()
throw()
{
	//std::cout << "memorypool destructor" << std::endl;
	
	s_BlockData* curr = m_availableBlocks;
	while (curr != 0) {
		s_BlockData* m_prev = curr->m_next;
		free(curr);
		curr = m_prev;
	}
	curr = m_fullBlocks;
	while (curr != 0) {
		s_BlockData* m_prev = curr->m_next;
		free(curr);
		curr = m_prev;
	}
	
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline typename MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::pointer
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::address(reference x)
const throw()
{
	return &x;
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline typename MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::const_pointer
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::address(const_reference x)
const throw()
{
	return &x;
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
void MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::removeBlockFromList(MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::s_BlockData **list, MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::s_BlockData* node)
{
	if (node->m_prev)
		node->m_prev->m_next = node->m_next;
	else
		*list = node->m_next;
	if (node->m_next)
		node->m_next->m_prev = node->m_prev;
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
void MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::moveToAnotherTop(
	s_BlockData**	to,
	s_BlockData**	from, 
	s_BlockData*	node)
{
	removeBlockFromList(from, node);
	if (*to)
	{
		node->m_next = *to;
		node->m_prev = NULL;
		(*to)->m_prev = node;
	}
	else
	{
		node->m_next = NULL;
		node->m_prev = NULL;
	}
	*to = node;
}

static void* requestaligned(size_t align, size_t size)
{
	void *result;
	if (posix_memalign(&result, align, size) != 0)
		throw std::bad_alloc();
	return (result);
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
void
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::allocateBlock()
{
	void *__newblock = requestaligned(BlockSize, BlockSize);
	
	t_data_pointer newBlock = static_cast<t_data_pointer>(__newblock);

	//std::cout << "allocating block at " << reinterpret_cast<void*>(newBlock) << std::endl;
	//<< " that ends at: " <<reinterpret_cast<void*>(newBlock + BlockSize) << std::endl;

	s_BlockData* block = reinterpret_cast<s_BlockData*>(newBlock);
	block->m_next = m_availableBlocks; // Previous head block becomes the m_next
	block->m_prev = NULL; // No previous block since this is the head


	if (m_availableBlocks)
		m_availableBlocks->m_prev = block; // Update previous block's link

	m_availableBlocks = block; // Update head block to the new one



	block->m_currentSlot = reinterpret_cast<t_slot_pointer>(block + 1);
	block->m_usedSlotsCount = 0;
	block->m_slotsCapacity = (BlockSize - (reinterpret_cast<size_t>(block->m_currentSlot) - reinterpret_cast<size_t>(newBlock))) / sizeof(t_slot_type);
	//std::cout << "						slot capacity: " << block->m_slotsCapacity << " vs this slot cap: " << block->m_slotsCapacity << std::endl;
	//std::cout << "diff: " << (reinterpret_cast<size_t>(block->m_currentSlot) - reinterpret_cast<size_t>(newBlock)) 
	//<< " vs sizeof(s_BlockData) " << sizeof(s_BlockData) << std::endl;
	block->m_freeSlots = NULL;
	this->m_freeBlocksCount++;
	//std::cout << "						freeblocks count (allocate block)" << this->m_freeBlocksCount << std::endl;	


/*
	t_data_pointer body = newBlock + sizeof(t_slot_pointer);
	size_type bodyPadding = padPointer(body, sizeof(t_slot_type));
	m_currentSlot = reinterpret_cast<t_slot_pointer>(body + bodyPadding);
	m_lastSlot = reinterpret_cast<t_slot_pointer>
	(newBlock + BlockSize - sizeof(t_slot_type) + 1);
*/

}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline typename MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::pointer
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::allocate(size_type, const_pointer)
{
	s_BlockData* 	block;
	pointer 	result;

	if (!m_availableBlocks)
		allocateBlock();
	block = m_availableBlocks;

	if (block->m_freeSlots != 0)
	{
		result = reinterpret_cast<pointer>(block->m_freeSlots);
		block->m_usedSlotsCount++;		
		block->m_freeSlots = block->m_freeSlots->m_next;
	}
	else
	{
		result = reinterpret_cast<pointer>(block->m_currentSlot);
		block->m_usedSlotsCount++;
		block->m_currentSlot++;
	}
	if (block->m_usedSlotsCount == block->m_slotsCapacity)
	{
		moveToAnotherTop(&m_fullBlocks, &m_availableBlocks, block);
	}
	//std::cout << "						slotCount: " << block->m_usedSlotsCount << " capacity:" << block->m_slotsCapacity << std::endl;
	if (block->m_usedSlotsCount == 1)
		this->m_freeBlocksCount--;
	//std::cout << "						freeblocks count (allocate element)" << this->m_freeBlocksCount << std::endl;			
	//std::cout << "allocating node at: " << result << ", at block " << block << std::endl;
	return (result);
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline void
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::deallocate(pointer p, size_type)
{
	
	if (p != 0)
	{
		//std::cout << "deallocating node at: " << p << std::endl;
		t_slot_pointer slot = reinterpret_cast<t_slot_pointer>(p);
		s_BlockData* block = reinterpret_cast<s_BlockData*>(reinterpret_cast<size_t>(slot) & (~(BlockSize - 1)));
		
		//std::cout << "deallocating node at: " << p << ", from block " << block << std::endl;
		slot->m_next = block->m_freeSlots;
		block->m_freeSlots = slot;

		block->m_usedSlotsCount--;
		if (!block->m_usedSlotsCount)
		{
			deallocateBlock(block);
			return ;
		}
		if (block->m_usedSlotsCount == block->m_slotsCapacity - 1) // it was full, no longer now
		{
			moveToAnotherTop(&m_availableBlocks, &m_fullBlocks, block);
		}
		
	}
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
void
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::deallocateBlock(s_BlockData* block)
{
	++this->m_freeBlocksCount;
	//std::cout << "maxFreeBlocks: " << m_maxFreeBlocks << ", vs freeblocks (deallocate block)" << this->m_freeBlocksCount<< std::endl;
	if (this->m_freeBlocksCount > m_maxFreeBlocks)
	{
		//std::cout << "						deallocating block " << reinterpret_cast<void*>(block) << std::endl;
		removeBlockFromList(&m_availableBlocks, block);

		// Deallocate the block
		free(block);

		this->m_freeBlocksCount--;

		//std::cout << " deleted?" << std::endl;
	}
	//std::cout << "finished deleting" << std::endl;
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline typename MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::size_type
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::max_size()
const throw()
{
	size_type maxBlocks = -1 / m_blockSize;
	return (m_blockSize - sizeof(t_data_pointer)) / sizeof(t_slot_type) * maxBlocks;
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline void
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::construct(pointer p, const_reference val)
{
	new (p) value_type(val);
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline void
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::destroy(pointer p)
{
	p->~value_type();
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline typename MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::pointer
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::newElement(const_reference val)
{
	pointer result = allocate();
	construct(result, val);
	return result;
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline void
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::deleteElement(pointer p)
{
	if (p != 0) {
		p->~value_type();
		deallocate(p);
	}
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::MemoryPool_AlignDealloc(const MemoryPool_AlignDealloc& memoryPool)
throw()
{
	(void)memoryPool;
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>& 
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::operator=(const MemoryPool_AlignDealloc& memoryPool)
{
	(void)memoryPool;
	return (*this);
}