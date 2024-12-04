/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nginx_MPool_FixedElem.tpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:49:34 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/04 09:20:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NGINX_MPOOL_FIXEDELEM_TPP
# define NGINX_MPOOL_FIXEDELEM_TPP

# include <limits>
# include <stdint.h>
# include <stddef.h>
# include <cstring>
# include <cassert>

# include <vector>
# include "../../../Arrays/Arrays.h"
# include "../Nginx_PoolAllocator.hpp"
# include "../../../CustomAssert/CustomAssert.hpp"

/*
	Template to get the element size aligned at compile time for correct vector allocation
*/



template <typename T>
class Nginx_MPool_FixedElem
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
			typedef Nginx_MPool_FixedElem<U> other;
		};

		Nginx_MPool_FixedElem(Nginx_MemoryPool* pool, size_t numElems = 0) throw();

		Nginx_MPool_FixedElem(const Nginx_MPool_FixedElem& memoryPool) throw();
		template <class U> Nginx_MPool_FixedElem(const Nginx_MPool_FixedElem<U>& memoryPool) throw();

		~Nginx_MPool_FixedElem() throw();

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
			char			m_data[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];
			s_Slot*			m_next;
		};

		typedef char*		t_data_pointer;
		typedef s_Slot		t_slot_type;
		typedef s_Slot*		t_slot_pointer;

		DynArray<s_Slot, Nginx_PoolAllocator<s_Slot> >	m_elements;
		size_t 											m_elemCount;
		size_t 											m_maxElems;
		t_slot_pointer 									m_freeSlot;

};


template <typename T>
Nginx_MPool_FixedElem<T>::Nginx_MPool_FixedElem(Nginx_MemoryPool* pool, size_t numElems) throw() :
	m_elements(0, Nginx_PoolAllocator<s_Slot>(pool)),
	m_elemCount(0),
	m_maxElems(numElems),
	m_freeSlot(NULL)
{
	//std::cout <<  "initializer: " << m_elements.getAllocator().m_memoryPool << "\n";
	//std::cout << "mem pool constructed: " << sizeof(T) << " array is size: " << m_elements.size() << std::endl;
}


template <typename T>
Nginx_MPool_FixedElem<T>::Nginx_MPool_FixedElem(const Nginx_MPool_FixedElem& memoryPool) throw() :
	m_elements(0, memoryPool.m_elements.getAllocator()),
	m_elemCount(memoryPool.m_elemCount),
	m_maxElems(memoryPool.m_maxElems),
	m_freeSlot(memoryPool.m_freeSlot)
{
	//std::cout <<  "copy constructor: " << memoryPool.m_elements.getAllocator().m_memoryPool << "\n";
	//m_elements = DynArray<s_Slot, Nginx_PoolAllocator<s_Slot> >(memoryPool.m_maxElems, m_elements.getAllocator());
	//std::cout << "mem pool copied: " << sizeof(T) << std::endl;
}



template <typename T>
template<class U>
Nginx_MPool_FixedElem<T>::Nginx_MPool_FixedElem(const Nginx_MPool_FixedElem<U>& memoryPool) throw() :
	m_elements(0, memoryPool.m_elements.getAllocator()),
	m_elemCount(0),
	m_maxElems(memoryPool.m_maxElems),
	m_freeSlot(NULL)
{

	//std::cout <<  "rebind: " << m_elements.getAllocator().m_memoryPool << "\n";
}



template <typename T>
Nginx_MPool_FixedElem<T>::~Nginx_MPool_FixedElem()
throw()
{
	//std::cout <<  " destructoed "<< std::endl;
}


template <typename T>
inline typename Nginx_MPool_FixedElem<T>::pointer
Nginx_MPool_FixedElem<T>::address(reference x)
const throw()
{
	return &x;
}



template <typename T>
inline typename Nginx_MPool_FixedElem<T>::const_pointer
Nginx_MPool_FixedElem<T>::address(const_reference x)
const throw()
{
	return &x;
}



template <typename T>
inline typename Nginx_MPool_FixedElem<T>::pointer
Nginx_MPool_FixedElem<T>::allocate(size_type, const_pointer)
{
	//std::cout << "allocate called sizeofT" << sizeof(T) << ".. max elems" << m_maxElems <<  "  array size" << m_elements.size() << std::endl;
	CUSTOM_ASSERT(m_elemCount < m_maxElems, "Nginx_MPool_FixedElem is at max capacity already");
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



template <typename T>
inline void
Nginx_MPool_FixedElem<T>::deallocate(pointer p, size_type)
{
	if (p != 0)
	{
		reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
		m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
		--m_elemCount;
	}
}

template <typename T>
inline typename Nginx_MPool_FixedElem<T>::size_type
Nginx_MPool_FixedElem<T>::max_size()
const throw()
{
	return (m_maxElems);
}



template <typename T>
inline void
Nginx_MPool_FixedElem<T>::construct(pointer p, const_reference val)
{
	//std::cout << "construct called sizeofT" << sizeof(T) << std::endl;
	new (p) value_type (val);
}



template <typename T>
inline void
Nginx_MPool_FixedElem<T>::destroy(pointer p)
{
	p->~value_type();
}



template <typename T>
inline typename Nginx_MPool_FixedElem<T>::pointer
Nginx_MPool_FixedElem<T>::newElement(const_reference val)
{
	pointer result = allocate();
	construct(result, val);
	return result;
}



template <typename T>
inline void
Nginx_MPool_FixedElem<T>::deleteElement(pointer p)
{
	if (p != 0)
	{
		p->~value_type();
		deallocate(p);
	}
}

#endif // MEMORY_BLOCK_TCC
