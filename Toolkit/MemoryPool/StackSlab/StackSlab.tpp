

#ifndef HEAPSLAB_TPP
# define HEAPSLABSLOT_TPP

// Project headers
# include "../../Arrays/StackArray/StackArray.hpp"
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <limits>
# include <cstddef>

class StackSlabImpl
{
	public:


		union s_Slot
		{
			s_Slot*		m_next;
		};

		typedef char*		t_data_pointer;
		typedef s_Slot		t_slot_type;
		typedef s_Slot*		t_slot_pointer;

		StackSlabImpl(t_slot_pointer array, size_t nodeSize, size_t capacity)
			: m_begin(array),
			m_nodeSize(nodeSize),
			m_elemCount(0),
			m_capacity(capacity),
			m_freeSlot(NULL) {}

		template <typename T>
		T* allocate()
		{
			ASSERT_EQUAL(sizeof(T) <= m_nodeSize, true, "HeapSlab: Size of T is greater than nodeSize");
			ASSERT_EQUAL(m_elemCount < m_capacity, true, "HeapSlab: Out of memory");

			if (m_freeSlot != NULL)
			{
				T* result = reinterpret_cast<T*>(m_freeSlot);
				m_freeSlot = m_freeSlot->m_next;
				m_elemCount++;
				return result;
			}
			else
			{
				return reinterpret_cast<T*>(reinterpret_cast<char*>(m_begin) + (m_nodeSize * m_elemCount++));
			}
		}

		template <typename T>
		void deallocate(T* p)
		{
			ASSERT_EQUAL(sizeof(T) <= m_nodeSize, true, "HeapSlab: Size of T is greater than nodeSize");

			if (p != NULL)
			{
				reinterpret_cast<t_slot_pointer>(p)->m_next = m_freeSlot;
				m_freeSlot = reinterpret_cast<t_slot_pointer>(p);
				--m_elemCount;
			}
		}

		size_t size() const { return m_elemCount; }
		size_t capacity() const { return m_capacity; }

	private:
		t_slot_pointer	m_begin;
		size_t			m_nodeSize;
		size_t			m_elemCount;
		size_t			m_capacity;
		t_slot_pointer	m_freeSlot;

		StackSlabImpl(const StackSlabImpl& memoryPool);
		StackSlabImpl& operator=(const StackSlabImpl& assign);
};

template <size_t nodeSize, size_t Capacity>
class StackSlab : public StackSlabImpl
{
	public:

		template <size_t TSize, size_t Alignment>
		struct AlignedSize
		{
			enum { value = (TSize + Alignment - 1) & ~(Alignment - 1) };
		};

		StackSlab()
			: StackSlabImpl(reinterpret_cast<t_slot_pointer>(m_byteArray), AlignedSize<nodeSize, sizeof(void*)>::value, Capacity) {}

		~StackSlab() {}

	private:
		char m_byteArray[Capacity * AlignedSize<nodeSize, sizeof(void*)>::value];


		StackSlab(const StackSlab& memoryPool) {(void)memoryPool;};
		StackSlab& operator=(const StackSlab& assign) {(void)assign; return (*this);};
};

#endif // MEMORY_BLOCK_TCC
