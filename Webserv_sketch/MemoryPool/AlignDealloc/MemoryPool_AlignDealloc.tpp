/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MemoryPool_AlignDealloc.tpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:47:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/25 12:08:47 by mmaria-d         ###   ########.fr       */
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

		struct BlockData;


		union  Slot_{
			char          	data_[sizeof(value_type)];
			Slot_*        	next;
		};

		
		typedef char* data_pointer_;
		typedef Slot_ slot_type_;
		typedef Slot_* slot_pointer_;

		struct BlockData {
			BlockData* 					next;
			BlockData* 					prev;
			size_t 						usedSlotsCount;
			size_t 						slotsCapacity;
			slot_pointer_   			currentSlot_;
			slot_pointer_   			freeSlots_;
		};

		size_t							blockSize_;
		BlockData*						availableBlocks_;
		BlockData*						fullBlocks_;
		size_t							freeBlocksCount_;
		size_t							maxFreeBlocks_;	
	

		size_type padPointer(data_pointer_ p, size_type align) const throw();
		void allocateBlock();
		void deallocateBlock(BlockData* block); // Deallocate a fully free block
		void moveToAnotherTop(BlockData**	to,BlockData**	from, BlockData*	node);
		void removeBlockFromList(BlockData **list, BlockData* node);

		MemoryPool_AlignDealloc(const MemoryPool_AlignDealloc& memoryPool) throw();
		MemoryPool_AlignDealloc& operator=(const MemoryPool_AlignDealloc& memoryPool);
};



template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline typename MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::size_type
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::padPointer(data_pointer_ p, size_type align)
const throw()
{
	size_t result = reinterpret_cast<size_t>(p);
	return ((align - result) % align);
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::MemoryPool_AlignDealloc() throw() : 
	blockSize_(BlockSize),
	availableBlocks_(0), 
	fullBlocks_(0),
	freeBlocksCount_(0), 
	maxFreeBlocks_(SpareBlocks)  // Allow up to 5 free blocks
{
    assert(((BlockSize & (BlockSize - 1)) == 0) && 
			BlockSize <= std::numeric_limits<uint16_t>::max() &&
			BlockSize >= 512); // Power of 2 check +
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::MemoryPool_AlignDealloc(size_t block_size, size_t starting_blocks, size_t spare_blocks) throw() :
	blockSize_(block_size),
	availableBlocks_(0), 
	fullBlocks_(0),
	freeBlocksCount_(0), 
	maxFreeBlocks_(spare_blocks)  // Allow up to 5 free blocks
{
    assert(((this->blockSize_ & (this->blockSize_ - 1)) == 0) && 
			this->blockSize_ <= std::numeric_limits<uint16_t>::max() &&
			this->blockSize_ >= 512 // Power of 2 check + page limits
			&& starting_blocks >= 0 && maxFreeBlocks_ >= starting_blocks);
	
	for (size_t i = 0; i < starting_blocks; i++)
		allocateBlock();
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
template<class U>
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::MemoryPool_AlignDealloc(const MemoryPool_AlignDealloc<U>& memoryPool)
throw() :
	blockSize_(memoryPool.blockSize_),
	availableBlocks_(0), 
	fullBlocks_(0),
	freeBlocksCount_(0), 
	maxFreeBlocks_(memoryPool.maxFreeBlocks_)
{
	
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::~MemoryPool_AlignDealloc()
throw()
{
    //std::cout << "memorypool destructor" << std::endl;
	
	BlockData* curr = availableBlocks_;
	while (curr != 0) {
		BlockData* prev = curr->next;
		free(curr);
		curr = prev;
	}
	curr = fullBlocks_;
	while (curr != 0) {
		BlockData* prev = curr->next;
		free(curr);
		curr = prev;
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
void MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::removeBlockFromList(MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::BlockData **list, MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::BlockData* node)
{
	if (node->prev)
		node->prev->next = node->next;
	else
		*list = node->next;
	if (node->next)
		node->next->prev = node->prev;
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
void MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::moveToAnotherTop(
	BlockData**	to,
	BlockData**	from, 
	BlockData*	node)
{
	removeBlockFromList(from, node);
	if (*to)
	{
		node->next = *to;
		node->prev = NULL;
		(*to)->prev = node;
	}
	else
	{
		node->next = NULL;
		node->prev = NULL;
	}
	*to = node;
}

void* requestaligned(size_t align, size_t size)
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
	
	data_pointer_ newBlock = static_cast<data_pointer_>(__newblock);

	//std::cout << "allocating block at " << reinterpret_cast<void*>(newBlock) << std::endl;
	//<< " that ends at: " <<reinterpret_cast<void*>(newBlock + BlockSize) << std::endl;

	BlockData* block = reinterpret_cast<BlockData*>(newBlock);
	block->next = availableBlocks_; // Previous head block becomes the next
	block->prev = NULL; // No previous block since this is the head


	if (availableBlocks_)
		availableBlocks_->prev = block; // Update previous block's link

	availableBlocks_ = block; // Update head block to the new one



    block->currentSlot_ = reinterpret_cast<slot_pointer_>(block + 1);
	block->usedSlotsCount = 0;
	block->slotsCapacity = (BlockSize - (reinterpret_cast<size_t>(block->currentSlot_) - reinterpret_cast<size_t>(newBlock))) / sizeof(slot_type_);
	//std::cout << "						slot capacity: " << block->slotsCapacity << " vs this slot cap: " << block->slotsCapacity << std::endl;
	//std::cout << "diff: " << (reinterpret_cast<size_t>(block->currentSlot_) - reinterpret_cast<size_t>(newBlock)) 
	//<< " vs sizeof(BlockData) " << sizeof(BlockData) << std::endl;
	block->freeSlots_ = NULL;
	this->freeBlocksCount_++;
	//std::cout << "						freeblocks count (allocate block)" << this->freeBlocksCount_ << std::endl;	


/*
	data_pointer_ body = newBlock + sizeof(slot_pointer_);
	size_type bodyPadding = padPointer(body, sizeof(slot_type_));
	currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
	lastSlot_ = reinterpret_cast<slot_pointer_>
	(newBlock + BlockSize - sizeof(slot_type_) + 1);
*/

}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline typename MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::pointer
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::allocate(size_type, const_pointer)
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
		moveToAnotherTop(&fullBlocks_, &availableBlocks_, block);
	}
	//std::cout << "						slotCount: " << block->usedSlotsCount << " capacity:" << block->slotsCapacity << std::endl;
	if (block->usedSlotsCount == 1)
		this->freeBlocksCount_--;
	//std::cout << "						freeblocks count (allocate element)" << this->freeBlocksCount_ << std::endl;			
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
		slot_pointer_ slot = reinterpret_cast<slot_pointer_>(p);
        BlockData* block = reinterpret_cast<BlockData*>(reinterpret_cast<size_t>(slot) & (~(BlockSize - 1)));
		
		//std::cout << "deallocating node at: " << p << ", from block " << block << std::endl;
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
			moveToAnotherTop(&availableBlocks_, &fullBlocks_, block);
		}
		
	}
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
void
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::deallocateBlock(BlockData* block)
{
	++this->freeBlocksCount_;
	//std::cout << "maxFreeBlocks: " << maxFreeBlocks_ << ", vs freeblocks (deallocate block)" << this->freeBlocksCount_<< std::endl;
    if (this->freeBlocksCount_ > maxFreeBlocks_)
    {
        //std::cout << "						deallocating block " << reinterpret_cast<void*>(block) << std::endl;
        removeBlockFromList(&availableBlocks_, block);

        // Deallocate the block
        free(block);

		this->freeBlocksCount_--;

        //std::cout << " deleted?" << std::endl;
	}
	//std::cout << "finished deleting" << std::endl;
}

template <typename T, size_t BlockSize, size_t StartingBlocks, size_t SpareBlocks>
inline typename MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::size_type
MemoryPool_AlignDealloc<T, BlockSize, StartingBlocks, SpareBlocks>::max_size()
const throw()
{
	size_type maxBlocks = -1 / blockSize_;
	return (blockSize_ - sizeof(data_pointer_)) / sizeof(slot_type_) * maxBlocks;
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