/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MemoryPoolDealloc.tpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:47:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/24 12:27:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <limits.h>
#include <stddef.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <stdint.h>

template <typename T, size_t BlockSize>
class MemoryPool
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
			typedef MemoryPool<U> other;
		};

		MemoryPool() throw();
		MemoryPool(const MemoryPool& memoryPool) throw();
		template <class U> MemoryPool(const MemoryPool<U, BlockSize>& memoryPool) throw();
		~MemoryPool() throw();

		pointer address(reference x) const throw();
		const_pointer address(const_reference x) const throw();

		pointer allocate(size_type n = 1, const_pointer hint = 0);
		void deallocate(pointer p, size_type n = 1);

		size_type max_size() const throw();

		void construct(pointer p, const_reference val);
		void destroy(pointer p);

		pointer newElement(const_reference val);
		void deleteElement(pointer p);

		struct BlockData;

		struct Slot_
		{
			union  {
				char          	data_[sizeof(value_type)];
				Slot_*        	next;
			};
			//BlockData*		  block_offset;
			uint16_t 			block_offset;
		};
		
		typedef char* data_pointer_;
		typedef Slot_ slot_type_;
		typedef Slot_* slot_pointer_;

		struct BlockData {
			BlockData* 					next;
			BlockData* 					prev;
			size_t 						usedSlotsCount;
			size_t						slotsCapacity;
			slot_pointer_   			currentSlot_;
			slot_pointer_   			freeSlots_;
		};

		BlockData*						availableBlocks_;
		BlockData*						fullBlocks_;

		size_t							freeBlocksCount_;
		size_t							maxFreeBlocks_;	
		//size_t						slotsCapacity_;

	private:

		size_type padPointer(data_pointer_ p, size_type align) const throw();

		void allocateBlock();
		
		void deallocateBlock(BlockData* block); // Deallocate a fully free block
};



template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::size_type
MemoryPool<T, BlockSize>::padPointer(data_pointer_ p, size_type align)
const throw()
{
	size_t result = reinterpret_cast<size_t>(p);
	return ((align - result) % align);
}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool() throw()
	: availableBlocks_(0), fullBlocks_(0),
	  freeBlocksCount_(0), maxFreeBlocks_(0)  // Allow up to 5 free blocks
{
    assert((BlockSize & (BlockSize - 1)) == 0); // Power of 2 check
}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool& memoryPool)
throw()
{
	(void)memoryPool;   // avoid unused parameter warning
	MemoryPool();
}

template <typename T, size_t BlockSize>
template<class U>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool<U, BlockSize>& memoryPool)
throw()
{
	(void)memoryPool;   // avoid unused parameter warning
	MemoryPool();
}

template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::~MemoryPool()
throw()
{
    //std::cout << "memorypool destructor" << std::endl;
	
	BlockData* curr = availableBlocks_;
	while (curr != 0) {
		BlockData* prev = curr->next;
		operator delete(reinterpret_cast<void*>(curr));
		curr = prev;
	}
	curr = fullBlocks_;
	while (curr != 0) {
		BlockData* prev = curr->next;
		operator delete(reinterpret_cast<void*>(curr));
		curr = prev;
	}
	
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::address(reference x)
const throw()
{
	return &x;
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::const_pointer
MemoryPool<T, BlockSize>::address(const_reference x)
const throw()
{
	return &x;
}



template <typename T, size_t BlockSize>
void
MemoryPool<T, BlockSize>::allocateBlock()
{
    
	data_pointer_ newBlock = static_cast<data_pointer_>(operator new(BlockSize));

	std::cout << "allocating block at " << reinterpret_cast<void*>(newBlock)
	<< " that ends at: " <<reinterpret_cast<void*>(newBlock + BlockSize) << std::endl;

	BlockData* block = reinterpret_cast<BlockData*>(newBlock);
	block->next = availableBlocks_; // Previous head block becomes the next
	block->prev = NULL; // No previous block since this is the head


	if (availableBlocks_)
		availableBlocks_->prev = block; // Update previous block's link

	availableBlocks_ = block; // Update head block to the new one



    data_pointer_ body = reinterpret_cast<data_pointer_>(block + 1);
    size_type bodyPadding = padPointer(body, sizeof(slot_type_));
    block->currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);

	block->slotsCapacity = ((BlockSize - sizeof(BlockData) + reinterpret_cast<size_t>(block) - reinterpret_cast<size_t>(block->currentSlot_)) / sizeof(slot_type_)); // Initially all slots are free
	block->usedSlotsCount = 0;
	
	//std::cout << "slotCount: " << block->freeSlotsCount << std::endl;
	block->freeSlots_ = NULL;
	this->freeBlocksCount_++;
	std::cout << "						freeblocks count (allocate block)" << this->freeBlocksCount_ << std::endl;	


/*
	data_pointer_ body = newBlock + sizeof(slot_pointer_);
	size_type bodyPadding = padPointer(body, sizeof(slot_type_));
	currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
	lastSlot_ = reinterpret_cast<slot_pointer_>
	(newBlock + BlockSize - sizeof(slot_type_) + 1);
*/

}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::allocate(size_type, const_pointer)
{
	BlockData* 	block;
	pointer 	result;

	if (!availableBlocks_)
		allocateBlock();
	block = availableBlocks_;

	if (block->freeSlots_ != 0)
	{
		result = reinterpret_cast<pointer>(block->freeSlots_);
		block->usedSlotsCount++;		
		block->freeSlots_ = block->freeSlots_->next;
	}
	else
	{
		result = reinterpret_cast<pointer>(block->currentSlot_);
        block->usedSlotsCount++;
		block->currentSlot_++;
	}
	if (block->usedSlotsCount == block->slotsCapacity)
	{
		block->freeSlots_ = NULL;
		if (block->next)
			block->next->prev = NULL;
		availableBlocks_ = block->next;

		block->next = fullBlocks_;
		if (fullBlocks_)
			fullBlocks_->prev = block;
		fullBlocks_ = block;
	}
	std::cout << "						slotCount: " << block->usedSlotsCount << " capacity:" << block->slotsCapacity << std::endl;
	if (block->usedSlotsCount == 1)
		this->freeBlocksCount_--;
	std::cout << "						freeblocks count (allocate element)" << this->freeBlocksCount_ << std::endl;			
	reinterpret_cast<slot_pointer_>(result)->block_offset = reinterpret_cast<size_t>(result) - reinterpret_cast<size_t>(block);
	std::cout << "allocating node at: " << result << ", at block " << block << std::endl;
	return (result);
}

template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::deallocate(pointer p, size_type)
{
    
	if (p != 0)
	{
		
		slot_pointer_ slot = reinterpret_cast<slot_pointer_>(p);
        BlockData* block = reinterpret_cast<BlockData*>(reinterpret_cast<size_t>(slot) - slot->block_offset);
		std::cout << "deallocating node at: " << p << ", from block " << block << std::endl;
		slot->next = block->freeSlots_;
		block->freeSlots_ = slot;

		block->usedSlotsCount--;
		if (!block->usedSlotsCount)
		{
			deallocateBlock(block);
			return ;
		}
		if (block->usedSlotsCount == block->slotsCapacity - 1) // it was full, no longer now
		{
			if (block->prev)
				block->prev->next = block->next;
			else
				fullBlocks_ = block->next;
				
			if (block->next)
				block->next->prev = block->prev;
			else
				fullBlocks_ = NULL;
			
			block->next = availableBlocks_;
			block->prev = NULL;
			availableBlocks_ = block;
		}
		
	}
}

template <typename T, size_t BlockSize>
void
MemoryPool<T, BlockSize>::deallocateBlock(BlockData* block)
{
	++this->freeBlocksCount_;
	std::cout << "maxFreeBlocks: " << maxFreeBlocks_ << ", vs freeblocks (deallocate block)" << this->freeBlocksCount_<< std::endl;
    if (this->freeBlocksCount_ > maxFreeBlocks_)
    {
        std::cout << "						deallocating block " << reinterpret_cast<void*>(block) << std::endl;
        if (block->prev)
		{
            block->prev->next = block->next;
        }
        else
            availableBlocks_ = block->next;

        if (block->next)
            block->next->prev = block->prev;
        else
			availableBlocks_ = NULL;

        // Deallocate the block
        operator delete(reinterpret_cast<void*>(block));

		this->freeBlocksCount_--;

        //std::cout << " deleted?" << std::endl;
	}
	//std::cout << "finished deleting" << std::endl;
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::size_type
MemoryPool<T, BlockSize>::max_size()
const throw()
{
	size_type maxBlocks = -1 / BlockSize;
	return (BlockSize - sizeof(data_pointer_)) / sizeof(slot_type_) * maxBlocks;
}

template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::construct(pointer p, const_reference val)
{
	new (p) value_type(val);
}

template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::destroy(pointer p)
{
	p->~value_type();
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::newElement(const_reference val)
{
	pointer result = allocate();
	construct(result, val);
	return result;
}

template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::deleteElement(pointer p)
{
	if (p != 0) {
		p->~value_type();
		deallocate(p);
	}
}
