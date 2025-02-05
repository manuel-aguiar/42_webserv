


#ifndef Stack_ObjectPool_TPP
#define Stack_ObjectPool_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"
#include "../../Arrays/StackArray/StackArray.hpp"

// C++ headers
#include <limits>
#include <cstddef>

/*
	Template to get the element size aligned at compile time for correct vector allocation
*/

template <typename T, size_t Size>
class Stack_ObjectPool
{
	public:

		template <size_t TSize, size_t Alignment>
		struct AlignedSize
		{
			static const size_t value = (TSize + Alignment - 1) & ~(Alignment - 1);
		};

		typedef T               value_type;
		typedef T*              pointer;
		typedef T&              reference;
		typedef const T*        const_pointer;
		typedef const T&        const_reference;
		typedef size_t          size_type;
		typedef ptrdiff_t       difference_type;

		Stack_ObjectPool() :
			m_elements(),
			m_elemCount(0),
			m_maxElems(Size),
			m_freeSlot(NULL) {}

		Stack_ObjectPool(const Stack_ObjectPool& copy)  :
			m_elements(0),
			m_elemCount(copy.m_elemCount),
			m_maxElems(copy.m_maxElems),
			m_freeSlot(copy.m_freeSlot) {}

		~Stack_ObjectPool() {}

		size_t		getElemCount() const { return (m_elemCount); }

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
			
			ASSERT_EQUAL(m_elemCount < m_maxElems, true, "Stack_ObjectPool: Out of memory");
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

		int getElementSize() const { return (sizeof(T)); }

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

		union s_Slot
		{
			char			m_data[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];
			s_Slot*			m_next;
		};

		typedef char*		t_data_pointer;
		typedef s_Slot		t_slot_type;
		typedef s_Slot*		t_slot_pointer;

		StackArray<s_Slot, Size> 	m_elements;
		size_t 						m_elemCount;
		size_t 						m_maxElems;
		t_slot_pointer 				m_freeSlot;

		bool operator==(const Stack_ObjectPool& other) const
		{ return (m_elements.getAllocator() == other.m_elements.getAllocator()); }
		bool operator!=(const Stack_ObjectPool& other) const
		{ return (m_elements.getAllocator() != other.m_elements.getAllocator()); }
};



#endif
