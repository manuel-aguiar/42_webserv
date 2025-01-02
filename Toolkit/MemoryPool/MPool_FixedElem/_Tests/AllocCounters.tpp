/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AllocCounters.tpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 09:01:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/27 13:22:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef ALLOCCOUNTERS_TPP

# define ALLOCCOUNTERS_TPP

# include "../MPool_FixedElem.hpp"
# include "../PoolAllocator_FixedElem.hpp"

# include "../../../List/List.hpp"
# include <list>

# include "../../../_Tests/test.h"

template <typename T>
class AllocCounter : public MPool_FixedElem<T>
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
			typedef AllocCounter<U> other;
		};

		AllocCounter(size_t numElems = 0, size_t* placeCounters = NULL) throw() : MPool_FixedElem<T>(numElems), m_placeCounters(placeCounters) {}
		AllocCounter(const AllocCounter& copy) throw() : MPool_FixedElem<T>(copy), m_placeCounters(copy.m_placeCounters) {}
		template <class U> AllocCounter(const AllocCounter<U>& rebind) throw() : MPool_FixedElem<T>(rebind), m_placeCounters(rebind.getAllocCounter()) {}
		~AllocCounter() throw() {}

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


template <typename T>
class PoolAllocCounter : public PoolAllocator_FixedElem<T>
{
	public:

		typedef T					value_type;
		typedef T*					pointer;
		typedef const T*			const_pointer;
		typedef T&					reference;
		typedef const T&			const_reference;
		typedef std::size_t			size_type;
		typedef std::ptrdiff_t		difference_type;

		template <typename U>
		struct rebind
		{
			typedef PoolAllocCounter<U> other;
		};

		PoolAllocCounter(MPool_FixedElem<T>& pool, size_t* placeCounters = NULL) : PoolAllocator_FixedElem<T>(pool), m_placeCounters(placeCounters) {}
		PoolAllocCounter(const PoolAllocCounter& copy) : PoolAllocator_FixedElem<T>(copy), m_placeCounters(copy.m_placeCounters) {}
		template <class U> PoolAllocCounter(const PoolAllocCounter<U>& rebind) throw() : PoolAllocator_FixedElem<T>(rebind), m_placeCounters(rebind.getAllocCounter()) {}
		~PoolAllocCounter() {}

		size_t* getAllocCounter() const { return m_placeCounters; }

		pointer allocate(size_type n, const void* hint = 0)
		{
			if (m_placeCounters != NULL)
				m_placeCounters[AllocCounter<T>::ALLOC]++;
			return PoolAllocator_FixedElem<T>::allocate(n, hint);
		}

		void deallocate(pointer p, size_type n)
		{
			if (m_placeCounters != NULL)
				m_placeCounters[AllocCounter<T>::DEALLOC]++;
			PoolAllocator_FixedElem<T>::deallocate(p, n);
		}

		void construct(pointer p, const_reference val)
		{
			PoolAllocator_FixedElem<T>::construct(p, val);
		}

		void destroy(pointer p)
		{
			PoolAllocator_FixedElem<T>::destroy(p);
		}

	private:
		size_t* m_placeCounters;
};

#endif