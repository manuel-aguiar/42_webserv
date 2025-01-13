/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heap_ObjectPool.tpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 00:28:05 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 00:35:27 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HEAP_OBJECTPOOL_TPP
#define HEAP_OBJECTPOOL_TPP

#include <limits>
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <cassert>

#include <vector>
#include "../../Arrays/DynArray/DynArray.hpp"

/*
	Template to get the element size aligned at compile time for correct vector allocation
*/

template <typename T, typename Allocator>
class Heap_ObjectPool
{
	public:

		template <size_t Size, size_t Alignment>
		struct AlignedSize
		{
			static const size_t value = (Size + Alignment - 1) & ~(Alignment - 1);
		};

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
			m_elements(0, SlotAllocator(allocator)),
			m_elemCount(0),
			m_maxElems(numElems),
			m_freeSlot(NULL) {}

		Heap_ObjectPool(const Heap_ObjectPool& copy)  :
			m_elements(0, copy.m_elements.getAllocator()),
			m_elemCount(copy.m_elemCount),
			m_maxElems(copy.m_maxElems),
			m_freeSlot(copy.m_freeSlot) {}

		template <class U> 
		Heap_ObjectPool(const Heap_ObjectPool<U, Allocator>& rebind) :
			m_elements(0, SlotAllocator(rebind.m_elements.getAllocator())),
			m_elemCount(0),
			m_maxElems(rebind.m_maxElems),
			m_freeSlot(NULL) {}

		~Heap_ObjectPool() {}

		pointer address(reference x) const
		{
			return (&x);
		}

		const_pointer address(const_reference x) const
		{
			return (&x);
		}

		pointer allocate(size_type n = 1, const_pointer hint = 0)
		{
			(void)hint;
			(void)n;
			
			assert(m_elemCount < m_maxElems);
			m_elements.reserve(m_maxElems);
			if (m_freeSlot != 0)
			{
				pointer result = reinterpret_cast<pointer>(m_freeSlot);
				m_freeSlot = m_freeSlot->m_next;
				m_elemCount++;
				return (result);
			}
			else
				return reinterpret_cast<pointer>(&m_elements[m_elemCount++]);
		}

		void deallocate(pointer p, size_type n = 1)
		{
			(void)n;
			if (p == NULL)
				return ;

			reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
			m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
			--m_elemCount;
		}

		size_type max_size() const
		{
			return (m_maxElems);
		}

		void construct(pointer p, const_reference val)
		{
			new (p) value_type (val);
		}
		
		void destroy(pointer p)
		{
			p->~value_type();
		}

		pointer newElement(const_reference val)
		{
			pointer result = allocate();
			construct(result, val);
			return result;
		}

		void deleteElement(pointer p)
		{
			if (p != 0)
			{
				p->~value_type();
				deallocate(p);
			}
		}

		const Allocator& getAllocator() const
		{
			return (typename Allocator::template rebind<T>::other(m_elements.getAllocator()));
		}

		union s_Slot
		{
			char			m_data[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];
			s_Slot*			m_next;
		};

		typedef char*		t_data_pointer;
		typedef s_Slot		t_slot_type;
		typedef s_Slot*		t_slot_pointer;
		typedef typename Allocator::template rebind<s_Slot>::other SlotAllocator;

		DynArray<s_Slot, SlotAllocator> 			
									m_elements;
		size_t 						m_elemCount;
		size_t 						m_maxElems;
		t_slot_pointer 				m_freeSlot;

		bool operator==(const Heap_ObjectPool& other) const
		{ return (m_elements.getAllocator() == other.m_elements.getAllocator()); }
		bool operator!=(const Heap_ObjectPool& other) const
		{ return (m_elements.getAllocator() != other.m_elements.getAllocator()); }
};



#endif
