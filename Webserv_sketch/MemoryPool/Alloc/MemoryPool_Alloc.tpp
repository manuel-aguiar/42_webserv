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
		// char          m_data[sizeof(value_type)]; is solved at compile time, so no overhead
		
		union s_Slot {
			//value_type   element;                      
			char          m_data[sizeof(value_type)];
			s_Slot*        m_next;
		};

		typedef char* t_data_pointer;
		typedef s_Slot t_slot_type;
		typedef s_Slot* t_slot_pointer;

		t_slot_pointer m_currentBlock;
		t_slot_pointer m_currentSlot;
		t_slot_pointer m_lastSlot;
		t_slot_pointer m_freeSlots;

		size_type padPointer(t_data_pointer p, size_type align) const throw();
		void allocateBlock();
};

template <typename T, size_t BlockSize>
inline typename MemoryPool_Alloc<T, BlockSize>::size_type
MemoryPool_Alloc<T, BlockSize>::padPointer(t_data_pointer p, size_type align)
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
	m_currentBlock = 0;
	m_currentSlot = 0;
	m_lastSlot = 0;
	m_freeSlots = 0;
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
	t_slot_pointer curr = m_currentBlock;
	while (curr != 0) {
		t_slot_pointer m_prev = curr->m_next;
		operator delete(reinterpret_cast<void*>(curr));
		curr = m_prev;
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
	t_data_pointer newBlock = reinterpret_cast<t_data_pointer>
													 (operator new(BlockSize));
	reinterpret_cast<t_slot_pointer>(newBlock)->m_next = m_currentBlock;
	m_currentBlock = reinterpret_cast<t_slot_pointer>(newBlock);
	t_data_pointer body = newBlock + sizeof(t_slot_pointer);
	size_type bodyPadding = padPointer(body, sizeof(t_slot_type));
	m_currentSlot = reinterpret_cast<t_slot_pointer>(body + bodyPadding);
	m_lastSlot = reinterpret_cast<t_slot_pointer>
							(newBlock + BlockSize - sizeof(t_slot_type) + 1);
}



template <typename T, size_t BlockSize>
inline typename MemoryPool_Alloc<T, BlockSize>::pointer
MemoryPool_Alloc<T, BlockSize>::allocate(size_type, const_pointer)
{
	 
	if (m_freeSlots != 0) {
		pointer result = reinterpret_cast<pointer>(m_freeSlots);
		m_freeSlots = m_freeSlots->m_next;
		//std::cout << "pool allocating: " << sizeof(T) << " at " << result << std::endl;
		return result;
	}
	else {
		if (m_currentSlot >= m_lastSlot)
			allocateBlock();
		//std::cout << "pool allocating: " << sizeof(T) << " at " << m_currentSlot << std::endl;
		return reinterpret_cast<pointer>(m_currentSlot++);
	}
}



template <typename T, size_t BlockSize>
inline void
MemoryPool_Alloc<T, BlockSize>::deallocate(pointer p, size_type)
{
	if (p != 0) {
		reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlots;
		m_freeSlots = reinterpret_cast<t_slot_pointer>(p);
	}
}



template <typename T, size_t BlockSize>
inline typename MemoryPool_Alloc<T, BlockSize>::size_type
MemoryPool_Alloc<T, BlockSize>::max_size()
const throw()
{
	size_type maxBlocks = -1 / BlockSize;
	return (BlockSize - sizeof(t_data_pointer)) / sizeof(t_slot_type) * maxBlocks;
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