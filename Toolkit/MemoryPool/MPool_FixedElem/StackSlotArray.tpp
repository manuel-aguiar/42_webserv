



#ifndef STACKSLOTARRAY_TCC
#define STACKSLOTARRAY_TCC

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
class StackSlotArray
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


		StackSlotArray() throw();

		~StackSlotArray();

		pointer allocate();

		pointer emplace();

		template<typename Arg1>
		pointer emplace(Arg1 arg1);

		template<typename Arg1, typename Arg2>
		pointer emplace(Arg1 arg1, Arg2 arg2);

		template<typename Arg1, typename Arg2, typename Arg3>
		pointer emplace(Arg1 arg1, Arg2 arg2, Arg3 arg3);

		void deallocate(pointer p);

		void destroy(pointer p);

		size_t size() const;

		union s_Slot
		{
			char		  	m_data[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];
			s_Slot*        	m_next;
		};

		typedef char*		t_data_pointer;
		typedef s_Slot		t_slot_type;
		typedef s_Slot*		t_slot_pointer;

		private:
			StackArray<s_Slot, Size> 	m_elements;
			size_t 						m_elemCount;
			size_t 						m_maxElems;
			t_slot_pointer 				m_freeSlot;
			
			StackSlotArray(const StackSlotArray& memoryPool) throw();
};

#include <iostream>
template <typename T, size_t Size>
StackSlotArray<T, Size>::StackSlotArray() throw() :
	m_elemCount(0),
	m_maxElems(Size),
	m_freeSlot(NULL)
{
	std::cout << "array starts at: " << &m_elements[0] << "  size " << Size << " sizeof T " << sizeof(T) << std::endl;
	//std::cout << "mem pool constructed: " << sizeof(T) << " array is size: " << m_elements.size() << std::endl;
}


template <typename T, size_t Size>
StackSlotArray<T, Size>::StackSlotArray(const StackSlotArray& copy) throw() :
	m_elements(0),
	m_elemCount(copy.m_elemCount),
	m_maxElems(Size),
	m_freeSlot(copy.m_freeSlot)
{
	//std::cout << "copy called" << std::endl;
	
	//std::cout << "mem pool copied: " << sizeof(T) << std::endl;
}


template <typename T, size_t Size>
size_t StackSlotArray<T, Size>::size() const
{
	return (m_elemCount);
}


template <typename T, size_t Size>
StackSlotArray<T, Size>::~StackSlotArray()
{
	//std::cout <<  " destructoed "<< std::endl;
}


template <typename T, size_t Size>
inline typename StackSlotArray<T, Size>::pointer
StackSlotArray<T, Size>::allocate()
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
		return (reinterpret_cast<pointer>(&m_elements[m_elemCount++]));
}



template <typename T, size_t Size>
inline void
StackSlotArray<T, Size>::deallocate(pointer p)
{
	if (p != 0)
	{
		reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
		m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
		--m_elemCount;
	}
}

template <typename T, size_t Size>
inline void
StackSlotArray<T, Size>::destroy(pointer p)
{
	if (!p)
		return ;
	p->~T();
	deallocate(p);
}

template <typename T, size_t Size>
inline typename StackSlotArray<T, Size>::pointer
StackSlotArray<T, Size>::emplace()
{
	pointer ptr;

	ptr = allocate();
	new (ptr) T();
	return (ptr);
}

template <typename T, size_t Size>
template<typename Arg1>
inline typename StackSlotArray<T, Size>::pointer
StackSlotArray<T, Size>::emplace(Arg1 arg1)
{
	(void)arg1;
	return ((pointer)1);
}

template <typename T, size_t Size>
template<typename Arg1, typename Arg2>
inline typename StackSlotArray<T, Size>::pointer
StackSlotArray<T, Size>::emplace(Arg1 arg1, Arg2 arg2)
{
	pointer ptr;

	ptr = allocate();
	new (ptr) T(arg1, arg2);
	return (ptr);
}

template <typename T, size_t Size>
template<typename Arg1, typename Arg2, typename Arg3>
inline typename StackSlotArray<T, Size>::pointer
StackSlotArray<T, Size>::emplace(Arg1 arg1, Arg2 arg2, Arg3 arg3)
{
	pointer ptr;

	ptr = allocate();
	new (ptr) T(arg1, arg2, arg3);
	return (ptr);
}


#endif // MEMORY_BLOCK_TCC
