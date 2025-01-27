

#ifndef HEAPSLAB_TPP
# define HEAPSLABSLOT_TPP

// Project headers
# include "../../Arrays/HeapArray/HeapArray.hpp"
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <limits>
# include <cstddef>

template <size_t nodeSize, typename Allocator>
class HeapSlab
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
    	HeapArray<s_Slot, typename Allocator::template rebind<s_Slot>::other>
												m_elements;

	public:
		HeapSlab(size_t capacity, const Allocator& allocator = Allocator()) :
			m_elemCount(0),
			m_freeSlot(NULL),
			m_elements(capacity, allocator) {};

		~HeapSlab() {};

		template <typename T>
		T* allocate()
		{
			ASSERT_EQUAL(sizeof(T) <= nodeSize, true, "HeapSlab: Size of T is greater than nodeSize");
			ASSERT_EQUAL(m_elemCount < m_elements.capacity(), true, "HeapSlab: Out of memory");

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

		const Allocator& getAllocator() const
		{
			return (m_elements.getAllocator());
		}

	private:
		HeapSlab(const HeapSlab& memoryPool) {(void)memoryPool;};
		HeapSlab& operator=(const HeapSlab& assign) {(void)assign; return(*this);};
};

#endif // MEMORY_BLOCK_TCC
