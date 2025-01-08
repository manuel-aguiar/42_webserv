/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StackSlotArray.tpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:21:01 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/08 11:27:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACKSLOTARRAY_TCC
# define STACKSLOTARRAY_TCC

// Project headers
# include "../../Arrays/StackArray/StackArray.hpp"

// C++ headers
# include <limits>
# include <cstddef>
# include <cassert>

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

		union s_Slot
		{
			char            m_data[AlignedSize<sizeof(value_type), __alignof__(value_type)>::value];
			s_Slot*         m_next;
		};

		typedef char*       t_data_pointer;
		typedef s_Slot      t_slot_type;
		typedef s_Slot*     t_slot_pointer;

	private:
		size_t                   m_elemCount;
		t_slot_pointer           m_freeSlot;
		StackArray<s_Slot, Size> m_elements;

	public:
		StackSlotArray() :
			m_elemCount(0),
			m_freeSlot(NULL) {};

		~StackSlotArray() {};

		pointer allocate()
		{
			assert(m_elemCount < Size);
			if (m_freeSlot != NULL)
			{
				pointer result = reinterpret_cast<pointer>(m_freeSlot);
				m_freeSlot = m_freeSlot->m_next;
				m_elemCount++;
				return result;
			}
			else
			{
				return reinterpret_cast<pointer>(&m_elements[m_elemCount++]);
			}
		}

		void deallocate(pointer p)
		{
			if (p != NULL)
			{
				reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
				m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
				--m_elemCount;
			}
		}

		void destroy(pointer p)
		{
			if (p == NULL)
				return;
			p->~T();
			deallocate(p);
		}

		pointer emplace()
		{
			pointer ptr = allocate();
			new (ptr) T();
			return ptr;
		}

		template<typename Arg1>
		pointer emplace(Arg1& arg1)
		{
			pointer ptr = allocate();
			new (ptr) T(arg1);
			return ptr;
		}

		template<typename Arg1, typename Arg2>
		pointer emplace(Arg1& arg1, Arg2& arg2)
		{
			pointer ptr = allocate();
			new (ptr) T(arg1, arg2);
			return ptr;
		}

		template<typename Arg1, typename Arg2, typename Arg3>
		pointer emplace(Arg1& arg1, Arg2& arg2, Arg3& arg3)
		{
			pointer ptr = allocate();
			new (ptr) T(arg1, arg2, arg3);
			return ptr;
		}

		template<typename Arg1>
		pointer emplace(const Arg1& arg1)
		{
			pointer ptr = allocate();
			new (ptr) T(arg1);
			return ptr;
		}

		template<typename Arg1, typename Arg2>
		pointer emplace(const Arg1& arg1, const Arg2& arg2)
		{
			pointer ptr = allocate();
			new (ptr) T(arg1, arg2);
			return ptr;
		}

		template<typename Arg1, typename Arg2, typename Arg3>
		pointer emplace(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
		{
			pointer ptr = allocate();
			new (ptr) T(arg1, arg2, arg3);
			return ptr;
		}

		size_t size() const
		{
			return m_elemCount;
		}

	private:
		StackSlotArray(const StackSlotArray& memoryPool) {(void)memoryPool;};
		StackSlotArray& operator=(const StackSlotArray& assign) {(void)assign; return(*this);};
};

#endif // MEMORY_BLOCK_TCC
