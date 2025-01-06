



#ifndef MPOOL_FIXEDELEMENTS_TCC
#define MPOOL_FIXEDELEMENTS_TCC

#include <limits>
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <cassert>

#include <vector>
#include "../../Arrays/StackArray/StackArray.hpp"

/*
	Template to get the element size aligned at compile time for correct vector allocation
*/

template <typename T, size_t Size>
class MPool_Stack
{
	public:

		template <size_t m_Size, size_t Alignment>
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


		MPool_Stack() throw();

		~MPool_Stack();

		pointer allocate();
		void deallocate(pointer p);

		size_t size() const;

		union s_Slot
		{
			char		  	m_data[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];
			s_Slot*        m_next;
		};

		typedef char*		t_data_pointer;
		typedef s_Slot		t_slot_type;
		typedef s_Slot*		t_slot_pointer;

		private:
			StackArray<s_Slot, Size> 	m_elements;
			size_t 						m_elemCount;
			size_t 						m_maxElems;
			t_slot_pointer 				m_freeSlot;
			
			MPool_Stack(const MPool_Stack& memoryPool) throw();
};


template <typename T, size_t Size>
MPool_Stack<T, Size>::MPool_Stack() throw() :
	m_elemCount(0),
	m_maxElems(Size),
	m_freeSlot(NULL)
{
	//std::cout << "mem pool constructed: " << sizeof(T) << " array is size: " << m_elements.size() << std::endl;
}


template <typename T, size_t Size>
MPool_Stack<T, Size>::MPool_Stack(const MPool_Stack& copy) throw() :
	m_elements(0),
	m_elemCount(copy.m_elemCount),
	m_maxElems(Size),
	m_freeSlot(copy.m_freeSlot)
{
	//std::cout << "copy called" << std::endl;
	
	//std::cout << "mem pool copied: " << sizeof(T) << std::endl;
}


template <typename T, size_t Size>
size_t MPool_Stack<T, Size>::size() const
{
	return (m_elemCount);
}


template <typename T, size_t Size>
MPool_Stack<T, Size>::~MPool_Stack()
{
	//std::cout <<  " destructoed "<< std::endl;
}


template <typename T, size_t Size>
inline typename MPool_Stack<T, Size>::pointer
MPool_Stack<T, Size>::allocate()
{
	//std::cout << "allocate called sizeofT" << sizeof(T) << ".. max elems" << m_maxElems <<  "  array size" << m_elements.size() << std::endl;
	assert(m_elemCount < m_maxElems);
	//std::cout << "allocate called" << std::endl;
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



template <typename T, size_t Size>
inline void
MPool_Stack<T, Size>::deallocate(pointer p)
{
	if (p != 0)
	{
		reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
		m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
		--m_elemCount;
	}
}

#endif // MEMORY_BLOCK_TCC
