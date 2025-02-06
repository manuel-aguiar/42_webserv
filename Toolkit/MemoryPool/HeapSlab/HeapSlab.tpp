

#ifndef HEAPSLAB_TPP
# define HEAPSLAB_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstddef>

// base class
# include "../SlabImpl/SlabImpl.hpp"

template <size_t nodeSize, typename Allocator>
class HeapSlab : public SlabImpl
{
	public:

		template <size_t TSize, size_t Alignment>
		struct AlignedSize
		{
			enum { value = (TSize + Alignment - 1) & ~(Alignment - 1) };
		};

		HeapSlab(size_t capacity = 0, const Allocator& allocator = Allocator()) : 
			SlabImpl(static_cast<t_slot_pointer>(NULL), 0, 0),
			m_allocator(typename Allocator::template rebind<char>::other(allocator)), 
			m_array(0)
		{
			initialize(capacity);
		}
		

		~HeapSlab()
		{
			if (m_array)
				m_allocator.deallocate(m_array, this->capacity() * AlignedSize<nodeSize, sizeof(void*)>::value);
		}


	private:
		Allocator 	m_allocator;
		char* 		m_array;

		void initialize(size_t capacity)
		{
			size_t alignedNodeSize = AlignedSize<nodeSize, sizeof(void*)>::value;
			m_array = m_allocator.allocate(capacity * alignedNodeSize);
			SlabImpl::reset(reinterpret_cast<t_slot_pointer>(m_array), alignedNodeSize, capacity);
		}


		HeapSlab(const HeapSlab &other) {(void)other;};
		HeapSlab &operator=(const HeapSlab &other) {(void)other; return (*this);};

};

#endif // MEMORY_BLOCK_TCC
