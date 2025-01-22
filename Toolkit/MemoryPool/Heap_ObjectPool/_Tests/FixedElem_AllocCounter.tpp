

# ifndef ALLOCCOUNTERS_TPP

# define ALLOCCOUNTERS_TPP

#include "../Heap_ObjectPool.hpp"

# include "../../../_Tests/TestHelpers.h"

template <typename T, typename Allocator = std::allocator<T> >
class FixedElem_AllocCounter : public Heap_ObjectPool<T, Allocator>
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
			typedef FixedElem_AllocCounter<U, Allocator> other;
		};

		FixedElem_AllocCounter(size_t numElems = 0, size_t* placeCounters = NULL) throw() : Heap_ObjectPool<T, Allocator>(numElems), m_placeCounters(placeCounters) {}
		FixedElem_AllocCounter(const FixedElem_AllocCounter& copy) throw() : Heap_ObjectPool<T, Allocator>(copy), m_placeCounters(copy.m_placeCounters) {}
		template <class U> FixedElem_AllocCounter(const FixedElem_AllocCounter<U, Allocator>& rebind) throw() : Heap_ObjectPool<T, Allocator>(rebind), m_placeCounters(rebind.getAllocCounter()) {}
		~FixedElem_AllocCounter() throw() {}

		size_t* getAllocCounter() const { return m_placeCounters; }

		pointer allocate(size_type n = 1, const_pointer hint = 0)
		{
			if (m_placeCounters != NULL)
				m_placeCounters[ALLOC]++;
			return Heap_ObjectPool<T, Allocator>::allocate(n, hint);
		}

		void deallocate(pointer p, size_type n = 1)
		{
			if (m_placeCounters != NULL)
				m_placeCounters[DEALLOC]++;
			Heap_ObjectPool<T, Allocator>::deallocate(p, n);
		}

	private:
		size_t* m_placeCounters;
};

#endif