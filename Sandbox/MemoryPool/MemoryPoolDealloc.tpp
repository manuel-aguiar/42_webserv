/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MemoryPoolDealloc.tpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:47:29 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/23 09:28:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <limits.h>
#include <stddef.h>
#include <cstring>
#include <iostream>

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

	// Block structure
    struct AlignedPtr {
        void* originalPtr;
        void* alignedPtr;
    };

	struct Block {
		Block* next;            // Next block in the list
		Block* prev;            // Previous block in the list
		size_t freeSlotsCount;  // Number of free slots in this block
        void*  unnalignedPtr;
		char data[1];           // Placeholder for actual slot data
	};

	private:

		union Slot_ {
			//value_type   element;                      
			char          data_[sizeof(value_type)];
			Slot_*        next;
		};


        
		typedef char* data_pointer_;
		typedef Slot_ slot_type_;
		typedef Slot_* slot_pointer_;

		Block*          currentBlock_;   // Head block (most recently allocated)
		slot_pointer_   currentSlot_;
		slot_pointer_   lastSlot_;
		slot_pointer_   freeSlots_; // Free slot list (single linked)

		size_t freeBlocksCount_;    // Number of fully freed blocks
		size_t maxFreeBlocks_; // Maximum number of free blocks allowed

		size_type padPointer(data_pointer_ p, size_type align) const throw();
		void allocateBlock();  // Allocate a new block
		void deallocateBlock(Block* block); // Deallocate a fully free block
        AlignedPtr aligned_alloc(size_t size, size_t alignment);
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
	: currentBlock_(0), currentSlot_(0), lastSlot_(0), freeSlots_(0),
	  freeBlocksCount_(0), maxFreeBlocks_(0) // Allow up to 5 free blocks
{
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
    std::cout << "memorypool destructor" << std::endl;
	Block* curr = currentBlock_;
	while (curr != 0) {
		Block* prev = curr->next;
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
inline typename MemoryPool<T, BlockSize>::AlignedPtr 
MemoryPool<T, BlockSize>::aligned_alloc(size_t size, size_t alignment)
{
    AlignedPtr ptrArr;

    // Allocate enough memory for alignment plus the requested size
    size_t totalSize = size + alignment - 1 + sizeof(void*);
    ptrArr.originalPtr = operator new(totalSize);
    
    if (!ptrArr.originalPtr) 
        return ((struct AlignedPtr){}); // Return null pointers if allocation fails

    // Calculate the aligned address
    ptrArr.alignedPtr = reinterpret_cast<void*>(
        (reinterpret_cast<size_t>(ptrArr.originalPtr) + sizeof(void*) + alignment - 1) & ~(alignment - 1)
    );

    // Store the original pointer just before the aligned pointer
    *(reinterpret_cast<void**>(ptrArr.alignedPtr) - 1) = ptrArr.originalPtr;

    return ptrArr;
}

template <typename T, size_t BlockSize>
void
MemoryPool<T, BlockSize>::allocateBlock()
{
       
	// Allocate memory for the block, including metadata and slots
    AlignedPtr ptr = aligned_alloc(BlockSize, BlockSize);
	data_pointer_ newBlock = static_cast<data_pointer_>(ptr.alignedPtr);

    std::cout << "allocating block at " << reinterpret_cast<void*>(newBlock) << std::endl;

	// Calculate the number of slots that fit in the block
	size_t slotsCount = (BlockSize - sizeof(Block)) / sizeof(slot_type_);

	// Initialize the block metadata
	Block* block = reinterpret_cast<Block*>(newBlock);
	block->next = currentBlock_; // Previous head block becomes the next
	block->prev = NULL; // No previous block since this is the head
	block->freeSlotsCount = slotsCount; // Initially all slots are free
    block->unnalignedPtr = ptr.originalPtr;


	if (currentBlock_) {
		currentBlock_->prev = block; // Update previous block's link
	}

	currentBlock_ = block; // Update head block to the new one

	// Set the current and last slots
	currentSlot_ = reinterpret_cast<slot_pointer_>(block + 1);
	lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_) + 1);
}

template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::allocate(size_type, const_pointer)
{
	if (freeSlots_ != 0) {
		// Allocate from free list
		pointer result = reinterpret_cast<pointer>(freeSlots_);
		freeSlots_ = freeSlots_->next;
        Block* block = reinterpret_cast<Block*>(
            reinterpret_cast<size_t>(result) & ~(BlockSize - 1)
        );
        block->freeSlotsCount--;
		return result;
	}
	else {
		if (currentSlot_ >= lastSlot_) {
			allocateBlock();
		}
        Block* block = reinterpret_cast<Block*>(
            reinterpret_cast<size_t>(currentSlot_) & ~(BlockSize - 1)
        );
        block->freeSlotsCount--;
		return reinterpret_cast<pointer>(currentSlot_++);
	}
}

template <typename T, size_t BlockSize>
inline void
MemoryPool<T, BlockSize>::deallocate(pointer p, size_type)
{
    
	if (p != 0) {
		slot_pointer_ slot = reinterpret_cast<slot_pointer_>(p);
		// Find the block containing the slot
        Block* block = reinterpret_cast<Block*>(
            reinterpret_cast<size_t>(p) & ~(BlockSize - 1)
        );
        //std::cout << "block is at: " << block << std::endl;

		// Add the slot back to the free list
		slot->next = freeSlots_;
		freeSlots_ = slot;

		// Update the block's free slot count
		block->freeSlotsCount++;
        std::cout << "free slots count: " << block->freeSlotsCount << ", total: " << (BlockSize - sizeof(Block)) / sizeof(slot_type_) << std::endl;
		// If the block is fully freed, deallocate the block
		if (block->freeSlotsCount == (BlockSize - sizeof(Block)) / sizeof(slot_type_)) {
			deallocateBlock(block);
		}
	}
}

template <typename T, size_t BlockSize>
void
MemoryPool<T, BlockSize>::deallocateBlock(Block* block)
{
    if (++freeBlocksCount_ > maxFreeBlocks_)
    {
        std::cout << "deallocating block " << reinterpret_cast<void*>(block) << std::endl;
        // Remove the block from the doubly linked list
        if (block->prev) {
            block->prev->next = block->next;
        }
        else {
            currentBlock_ = block->next;
        }

        if (block->next) {
            block->next->prev = block->prev;
        }

        // Deallocate the block
        operator delete(reinterpret_cast<void*>(block->unnalignedPtr));
        std::cout << " deleted?" << std::endl;
	}
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
