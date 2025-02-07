


#ifndef HEAP_OBJECTPOOL_TPP
#define HEAP_OBJECTPOOL_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"
#include "../../Arrays/DynArray/DynArray.hpp"

// C++ headers
#include <limits>
#include <cstddef>
#include <cstring>
#include <cassert>

/*
	Template to get the element size aligned at compile time for correct vector allocation
*/

// Base class
#include "../_ObjectPoolImpl/ObjectPoolImpl.hpp"

template <typename T, typename Allocator>
class Heap_ObjectPool : public ObjectPoolImpl<T>
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
			typedef Heap_ObjectPool<U, Allocator> other;
		};

		Heap_ObjectPool(size_t numElems, const Allocator& allocator = Allocator()) :
			ObjectPoolImpl<T>(NULL, numElems),
			m_elements(0, SlotAllocator(allocator)) {}

		Heap_ObjectPool(const Heap_ObjectPool& copy)  :
			ObjectPoolImpl<T>(copy),
			m_elements(0, copy.m_elements.getAllocator()) {}

		template <class U> 
		Heap_ObjectPool(const Heap_ObjectPool<U, Allocator>& rebind) :
			ObjectPoolImpl<T>(rebind),
			m_elements(0, SlotAllocator(rebind.m_elements.getAllocator())) {}

		~Heap_ObjectPool() {}

		pointer allocate(size_type n = 1, const_pointer hint = 0)
		{
			(void)hint;
			(void)n;
			m_elements.reserve(this->capacity());
			ObjectPoolImpl<T>::assignArray(m_elements.getArray());
			return (ObjectPoolImpl<T>::allocate(n, hint));
		}


		const Allocator& getAllocator() const
		{
			return (m_elements.getAllocator());
		}

		typedef typename Allocator::template rebind<typename ObjectPoolImpl<T>::s_Slot>::other SlotAllocator;

		DynArray<typename ObjectPoolImpl<T>::s_Slot, SlotAllocator> 			
									m_elements;

		bool operator==(const Heap_ObjectPool& other) const
		{ return (m_elements.getAllocator() == other.m_elements.getAllocator()); }
		bool operator!=(const Heap_ObjectPool& other) const
		{ return (m_elements.getAllocator() != other.m_elements.getAllocator()); }
};



#endif
