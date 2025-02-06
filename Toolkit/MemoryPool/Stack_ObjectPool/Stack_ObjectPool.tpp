


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


template <typename T>
class Stack_ObjectPoolImpl
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


		union s_Slot
		{
			char			m_data[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];
			s_Slot*			m_next;
		};

		typedef char*		t_data_pointer;
		typedef s_Slot		t_slot_type;
		typedef s_Slot*		t_slot_pointer;

		Stack_ObjectPoolImpl(t_slot_pointer begin, size_t maxElems) :
			m_begin(begin),
			m_elemCount(0),
			m_maxElems(maxElems),
			m_freeSlot(NULL) {}

		Stack_ObjectPoolImpl(const Stack_ObjectPoolImpl& copy)  :
			m_begin(copy.m_begin),
			m_elemCount(copy.m_elemCount),
			m_maxElems(copy.m_maxElems),
			m_freeSlot(copy.m_freeSlot) {}

		~Stack_ObjectPoolImpl() {}

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
			
			ASSERT_EQUAL(m_elemCount < m_maxElems, true, "Stack_ObjectPoolImpl: Out of memory");
			if (m_freeSlot != 0)
			{
				pointer result = reinterpret_cast<pointer>(m_freeSlot);
				m_freeSlot = m_freeSlot->m_next;
				m_elemCount++;
				return (result);
			}
			else
				return reinterpret_cast<pointer>(&m_begin[m_elemCount++]);
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


		t_slot_pointer 				m_begin;
		size_t 						m_elemCount;
		size_t 						m_maxElems;
		t_slot_pointer 				m_freeSlot;

		bool operator==(const Stack_ObjectPoolImpl& other) const
		{ return (m_begin == other.m_begin); }
		bool operator!=(const Stack_ObjectPoolImpl& other) const
		{ return (m_begin != other.m_begin); }
};

template <typename T, size_t Size>
class Stack_ObjectPool : public Stack_ObjectPoolImpl<T>
{
	public:
		Stack_ObjectPool() : 
			Stack_ObjectPoolImpl<T>(m_storage.getArray(), Size) {}

		Stack_ObjectPool(const Stack_ObjectPool &other) :
			Stack_ObjectPoolImpl<T>(other)
		{
			*this = other;
		}

		~Stack_ObjectPool() {}

		Stack_ObjectPool &operator=(const Stack_ObjectPool &other)
		{
			if (this == &other)
				return (*this);
			
			Stack_ObjectPoolImpl<T>::operator=(other);

			return (*this);
		}
	
	private:
		StackArray<typename Stack_ObjectPoolImpl<T>::t_slot_type, Size> m_storage;
};



#endif
