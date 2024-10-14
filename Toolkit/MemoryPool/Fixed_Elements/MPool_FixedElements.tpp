



#ifndef MPOOL_FIXEDELEMENTS_TCC
#define MPOOL_FIXEDELEMENTS_TCC

#include <limits>
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <cassert>

#include <vector>
#include "../../Arrays/Arrays.h"

/*
	Template to get the element size aligned at compile time for correct vector allocation
*/
template <size_t Size, size_t Alignment>
struct AlignedSize
{
    static const size_t value = (Size + Alignment - 1) & ~(Alignment - 1);
};


template <typename T>
class MPool_FixedElem
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
			typedef MPool_FixedElem<U> other;
		};

		MPool_FixedElem(size_t numElems = 0) throw();

		MPool_FixedElem(const MPool_FixedElem& memoryPool) throw();
		template <class U> MPool_FixedElem(const MPool_FixedElem<U>& memoryPool) throw();

		~MPool_FixedElem() throw();

		pointer address(reference x) const throw();
		const_pointer address(const_reference x) const throw();

		pointer allocate(size_type n = 1, const_pointer hint = 0);
		void deallocate(pointer p, size_type n = 1);

		size_type max_size() const throw();

		void construct(pointer p, const_reference val);
		void destroy(pointer p);

		pointer newElement(const_reference val);
		void deleteElement(pointer p);

		union s_Slot
		{
			char		  	m_data[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];                      
			s_Slot*        m_next;
		};

		typedef char*		t_data_pointer;
		typedef s_Slot		t_slot_type;
		typedef s_Slot*		t_slot_pointer;

		HeapArray<s_Slot> 	m_elements;
		size_t 				m_elemCount;
		size_t 				m_maxElems;
		t_slot_pointer 		m_freeSlot;

};


template <typename T>
MPool_FixedElem<T>::MPool_FixedElem(size_t numElems) throw() :
	m_elements(numElems),
	m_elemCount(0),
	m_maxElems(numElems),
	m_freeSlot(NULL)
{
	//std::cout << "mem pool constructed: " << sizeof(T) << " array is size: " << m_elements.size() << std::endl;
}


template <typename T>
MPool_FixedElem<T>::MPool_FixedElem(const MPool_FixedElem& memoryPool) throw() :
	m_elemCount(memoryPool.m_elemCount),
	m_maxElems(memoryPool.m_maxElems),
	m_freeSlot(memoryPool.m_freeSlot)
{
	m_elements = HeapArray<s_Slot>(memoryPool.m_maxElems);
	//std::cout << "mem pool copied: " << sizeof(T) << std::endl;
}



template <typename T>
template<class U>
MPool_FixedElem<T>::MPool_FixedElem(const MPool_FixedElem<U>& memoryPool) throw() :
	m_elemCount(0),
	m_maxElems(memoryPool.m_maxElems),
	m_freeSlot(NULL)
{
	std::cout << "rebind: " << sizeof(T) << std::endl;
}



template <typename T>
MPool_FixedElem<T>::~MPool_FixedElem()
throw()
{
	//std::cout <<  " destructoed "<< std::endl;
}


template <typename T>
inline typename MPool_FixedElem<T>::pointer
MPool_FixedElem<T>::address(reference x)
const throw()
{
	return &x;
}



template <typename T>
inline typename MPool_FixedElem<T>::const_pointer
MPool_FixedElem<T>::address(const_reference x)
const throw()
{
	return &x;
}



template <typename T>
inline typename MPool_FixedElem<T>::pointer
MPool_FixedElem<T>::allocate(size_type, const_pointer)
{
	//std::cout << "allocate called sizeofT" << sizeof(T) << ".. max elems" << m_maxElems <<  "  array size" << m_elements.size() << std::endl; 
	assert(m_elemCount < m_maxElems);
	
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



template <typename T>
inline void
MPool_FixedElem<T>::deallocate(pointer p, size_type)
{
	if (p != 0)
	{
		reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
		m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
		--m_elemCount;
	}
}

template <typename T>
inline typename MPool_FixedElem<T>::size_type
MPool_FixedElem<T>::max_size()
const throw()
{
	return (m_maxElems);
}



template <typename T>
inline void
MPool_FixedElem<T>::construct(pointer p, const_reference val)
{
	//std::cout << "construct called sizeofT" << sizeof(T) << std::endl; 
	new (p) value_type (val);
}



template <typename T>
inline void
MPool_FixedElem<T>::destroy(pointer p)
{
	p->~value_type();
}



template <typename T>
inline typename MPool_FixedElem<T>::pointer
MPool_FixedElem<T>::newElement(const_reference val)
{
	pointer result = allocate();
	construct(result, val);
	return result;
}



template <typename T>
inline void
MPool_FixedElem<T>::deleteElement(pointer p)
{
	if (p != 0)
	{
		p->~value_type();
		deallocate(p);
	}
}

#endif // MEMORY_BLOCK_TCC