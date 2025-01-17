/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StackSlab.tpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:21:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 11:27:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAPSLAB_TPP
# define HEAPSLABSLOT_TPP

// Project headers
# include "../../Arrays/StackArray/StackArray.hpp"

// C++ headers
# include <limits>
# include <cstddef>
# include <cassert>

template <size_t nodeSize, size_t Capacity>
class StackSlab
{
	public:
		template <size_t TSize, size_t Alignment>
		struct AlignedSize
		{
			static const size_t value = (TSize + Alignment - 1) & ~(Alignment - 1);
		};

		union s_Slot
		{
			char            m_data[AlignedSize<nodeSize, __alignof__(nodeSize)>::value];
			s_Slot*         m_next;
		};

		typedef char*       t_data_pointer;
		typedef s_Slot      t_slot_type;
		typedef s_Slot*     t_slot_pointer;

	private:
		size_t									m_elemCount;
		t_slot_pointer							m_freeSlot;
    	StackArray<s_Slot, Capacity>			m_elements;

	public:
		StackSlab() :
			m_elemCount(0),
			m_freeSlot(NULL) {};

		~StackSlab() {};

		template <typename T>
		T* allocate()
		{

			assert(sizeof(T) <= nodeSize);
			assert(m_elemCount < m_elements.capacity());

			if (m_freeSlot != NULL)
			{
				T* result = reinterpret_cast<T*>(m_freeSlot);
				m_freeSlot = m_freeSlot->m_next;
				m_elemCount++;
				return result;
			}
			else
				return (reinterpret_cast<T*>(&m_elements[m_elemCount++]));
		}

		template <typename T>
		void deallocate(T* p)
		{
			assert(sizeof(T) <= nodeSize);

			if (p != NULL)
			{
				reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
				m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
				--m_elemCount;
			}
		}

		size_t size() const
		{
			return (m_elemCount);
		}

		size_t capacity() const
		{
			return (m_elements.capacity());
		}

	private:
		StackSlab(const StackSlab& memoryPool) {(void)memoryPool;};
		StackSlab& operator=(const StackSlab& assign) {(void)assign; return(*this);};
};

#endif // MEMORY_BLOCK_TCC
