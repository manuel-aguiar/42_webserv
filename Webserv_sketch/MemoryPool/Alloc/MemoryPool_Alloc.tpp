/*-
 * Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/*

	This is the same code as defined in the license above. Changes i applied are commented in the code

	Indentation adjusted for taste

	I took it and adapted it to create a SharedMPool_Alloc class that uses a reference count to manage the memory pool
	and allow sharing between different containers. (see main)

	This pool only "works" for objects without random access iterators:
		elemets will be scattered everywhere......, so this works with std::list std::map and std::set

		Anyways, vectors are arrays already, so they don't need this kind of memory pool

*/



#ifndef MEMORY_BLOCK_TCC
#define MEMORY_BLOCK_TCC

#include <limits>
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <cassert>


template <typename T, size_t BlockSize>
class MemoryPool_Alloc
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
			typedef MemoryPool_Alloc<U> other;
		};

		MemoryPool_Alloc() throw();
		MemoryPool_Alloc(const MemoryPool_Alloc& memoryPool) throw();
		template <class U> MemoryPool_Alloc(const MemoryPool_Alloc<U, BlockSize>& memoryPool) throw();

		~MemoryPool_Alloc() throw();

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

		// small change to allow non-trivial objects to be constructed, instead of value_type, char[sizeof(value_type)] is used
		// unions in c++98 don't allow non-trivial objects
		// char          data_[sizeof(value_type)]; is solved at compile time, so no overhead
		
		union Slot_ {
			//value_type   element;                      
			char          data_[sizeof(value_type)];
			Slot_*        next;
		};

		typedef char* data_pointer_;
		typedef Slot_ slot_type_;
		typedef Slot_* slot_pointer_;

		slot_pointer_ currentBlock_;
		slot_pointer_ currentSlot_;
		slot_pointer_ lastSlot_;
		slot_pointer_ freeSlots_;

		size_type padPointer(data_pointer_ p, size_type align) const throw();
		void allocateBlock();
};

template <typename T, size_t BlockSize>
inline typename MemoryPool_Alloc<T, BlockSize>::size_type
MemoryPool_Alloc<T, BlockSize>::padPointer(data_pointer_ p, size_type align)
const throw()
{
	size_t result = reinterpret_cast<size_t>(p);
	return ((align - result) % align);
}



template <typename T, size_t BlockSize>
MemoryPool_Alloc<T, BlockSize>::MemoryPool_Alloc()
throw()
{
	//std::cout << "memory pool constructed" << std::endl;
    assert(((BlockSize & (BlockSize - 1)) == 0) && 
			BlockSize <= std::numeric_limits<uint16_t>::max() &&
			BlockSize >= 512); // Power of 2 check + page limits
	currentBlock_ = 0;
	currentSlot_ = 0;
	lastSlot_ = 0;
	freeSlots_ = 0;
}



template <typename T, size_t BlockSize>
MemoryPool_Alloc<T, BlockSize>::MemoryPool_Alloc(const MemoryPool_Alloc& memoryPool)
throw()
{
	(void)memoryPool;   // avoid unused parameter warning
	MemoryPool_Alloc();
}



template <typename T, size_t BlockSize>
template<class U>
MemoryPool_Alloc<T, BlockSize>::MemoryPool_Alloc(const MemoryPool_Alloc<U, BlockSize>& memoryPool)
throw()
{
	(void)memoryPool;   // avoid unused parameter warning
	MemoryPool_Alloc();
}



template <typename T, size_t BlockSize>
MemoryPool_Alloc<T, BlockSize>::~MemoryPool_Alloc()
throw()
{
	slot_pointer_ curr = currentBlock_;
	while (curr != 0) {
		slot_pointer_ prev = curr->next;
		operator delete(reinterpret_cast<void*>(curr));
		curr = prev;
	}
}



template <typename T, size_t BlockSize>
inline typename MemoryPool_Alloc<T, BlockSize>::pointer
MemoryPool_Alloc<T, BlockSize>::address(reference x)
const throw()
{
	return &x;
}



template <typename T, size_t BlockSize>
inline typename MemoryPool_Alloc<T, BlockSize>::const_pointer
MemoryPool_Alloc<T, BlockSize>::address(const_reference x)
const throw()
{
	return &x;
}



template <typename T, size_t BlockSize>
void
MemoryPool_Alloc<T, BlockSize>::allocateBlock()
{
	data_pointer_ newBlock = reinterpret_cast<data_pointer_>
													 (operator new(BlockSize));
	reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
	currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
	data_pointer_ body = newBlock + sizeof(slot_pointer_);
	size_type bodyPadding = padPointer(body, sizeof(slot_type_));
	currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
	lastSlot_ = reinterpret_cast<slot_pointer_>
							(newBlock + BlockSize - sizeof(slot_type_) + 1);
}



template <typename T, size_t BlockSize>
inline typename MemoryPool_Alloc<T, BlockSize>::pointer
MemoryPool_Alloc<T, BlockSize>::allocate(size_type, const_pointer)
{
	 
	if (freeSlots_ != 0) {
		pointer result = reinterpret_cast<pointer>(freeSlots_);
		freeSlots_ = freeSlots_->next;
		//std::cout << "pool allocating: " << sizeof(T) << " at " << result << std::endl;
		return result;
	}
	else {
		if (currentSlot_ >= lastSlot_)
			allocateBlock();
		//std::cout << "pool allocating: " << sizeof(T) << " at " << currentSlot_ << std::endl;
		return reinterpret_cast<pointer>(currentSlot_++);
	}
}



template <typename T, size_t BlockSize>
inline void
MemoryPool_Alloc<T, BlockSize>::deallocate(pointer p, size_type)
{
	if (p != 0) {
		reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
		freeSlots_ = reinterpret_cast<slot_pointer_>(p);
	}
}



template <typename T, size_t BlockSize>
inline typename MemoryPool_Alloc<T, BlockSize>::size_type
MemoryPool_Alloc<T, BlockSize>::max_size()
const throw()
{
	size_type maxBlocks = -1 / BlockSize;
	return (BlockSize - sizeof(data_pointer_)) / sizeof(slot_type_) * maxBlocks;
}



template <typename T, size_t BlockSize>
inline void
MemoryPool_Alloc<T, BlockSize>::construct(pointer p, const_reference val)
{
	//std::cout << "pool CONSTRUCTING: " << sizeof(T) << " at " << p << std::endl; 
	new (p) value_type (val);
	//std::cout << "MemoryPool_Alloc::construct" << std::endl;    
}



template <typename T, size_t BlockSize>
inline void
MemoryPool_Alloc<T, BlockSize>::destroy(pointer p)
{
	p->~value_type();
	//std::cout << "MemoryPool_Alloc::destroy" << std::endl;
}



template <typename T, size_t BlockSize>
inline typename MemoryPool_Alloc<T, BlockSize>::pointer
MemoryPool_Alloc<T, BlockSize>::newElement(const_reference val)
{
	pointer result = allocate();
	construct(result, val);
	return result;
}



template <typename T, size_t BlockSize>
inline void
MemoryPool_Alloc<T, BlockSize>::deleteElement(pointer p)
{
	if (p != 0) {
		p->~value_type();
		deallocate(p);
	}
}



#endif // MEMORY_BLOCK_TCC