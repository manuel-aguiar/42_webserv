/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FixedElem_AllocCounter.tpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:01:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/11 01:05:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef ALLOCCOUNTERS_TPP

# define ALLOCCOUNTERS_TPP

#include "../Stack_ObjectPool.hpp"

# include "../../../_Tests/test.h"

template <typename T, size_t Size>
class FixedElem_AllocCounter : public Stack_ObjectPool<T, Size>
{
	public:

		enum counterType
		{
			ALLOC = 0,
			DEALLOC,
			COUNT,
		};

		typedef T               value_type;
		typedef T*              pointer;
		typedef T&              reference;
		typedef const T*        const_pointer;
		typedef const T&        const_reference;
		typedef size_t          size_type;
		typedef ptrdiff_t       difference_type;


		FixedElem_AllocCounter(size_t numElems = 0, size_t* placeCounters = NULL) throw() : Stack_ObjectPool<T, Size>(numElems), m_placeCounters(placeCounters) {}
		FixedElem_AllocCounter(const FixedElem_AllocCounter& copy) throw() : Stack_ObjectPool<T, Size>(copy), m_placeCounters(copy.m_placeCounters) {}
		~FixedElem_AllocCounter() throw() {}

		size_t* getAllocCounter() const { return m_placeCounters; }

		pointer allocate(size_type n = 1, const_pointer hint = 0)
		{
			if (m_placeCounters != NULL)
				m_placeCounters[ALLOC]++;
			return Stack_ObjectPool<T, Size>::allocate(n, hint);
		}

		void deallocate(pointer p, size_type n = 1)
		{
			if (m_placeCounters != NULL)
				m_placeCounters[DEALLOC]++;
			Stack_ObjectPool<T, Size>::deallocate(p, n);
		}

	private:
		size_t* m_placeCounters;
};

#endif