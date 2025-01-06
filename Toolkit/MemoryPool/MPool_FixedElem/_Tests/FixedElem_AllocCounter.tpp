/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MPool_FixedElem_AllocCounter.tpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:01:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/06 00:31:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef ALLOCCOUNTERS_TPP

# define ALLOCCOUNTERS_TPP

# include "../MPool_FixedElem.hpp"

# include "../../../_Tests/test.h"

template <typename T>
class FixedElem_AllocCounter : public MPool_FixedElem<T>
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

		template <typename U> struct rebind {
			typedef FixedElem_AllocCounter<U> other;
		};

		FixedElem_AllocCounter(size_t numElems = 0, size_t* placeCounters = NULL) throw() : MPool_FixedElem<T>(numElems), m_placeCounters(placeCounters) {}
		FixedElem_AllocCounter(const FixedElem_AllocCounter& copy) throw() : MPool_FixedElem<T>(copy), m_placeCounters(copy.m_placeCounters) {}
		template <class U> FixedElem_AllocCounter(const FixedElem_AllocCounter<U>& rebind) throw() : MPool_FixedElem<T>(rebind), m_placeCounters(rebind.getAllocCounter()) {}
		~FixedElem_AllocCounter() throw() {}

		size_t* getAllocCounter() const { return m_placeCounters; }

		pointer allocate(size_type n = 1, const_pointer hint = 0)
		{
			if (m_placeCounters != NULL)
				m_placeCounters[ALLOC]++;
			return MPool_FixedElem<T>::allocate(n, hint);
		}

		void deallocate(pointer p, size_type n = 1)
		{
			if (m_placeCounters != NULL)
				m_placeCounters[DEALLOC]++;
			MPool_FixedElem<T>::deallocate(p, n);
		}

	private:
		size_t* m_placeCounters;
};

#endif