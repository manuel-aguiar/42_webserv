

#ifndef STACKSLAB_TPP
# define STACKSLAB_TPP

// Project headers
# include "../../Arrays/StackArray/StackArray.hpp"
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstddef>


//Base class
# include "../_SlabImpl/SlabImpl.hpp"

template <size_t nodeSize, size_t Capacity>
class StackSlab : public SlabImpl
{
	public:

		template <size_t TSize, size_t Alignment>
		struct AlignedSize
		{
			enum { value = (TSize + Alignment - 1) & ~(Alignment - 1) };
		};

		StackSlab()
			: SlabImpl(reinterpret_cast<t_slot_pointer>(m_byteArray), AlignedSize<nodeSize, sizeof(void*)>::value, Capacity) {}

		~StackSlab() {}

	private:
		char m_byteArray[Capacity * AlignedSize<nodeSize, sizeof(void*)>::value];


		StackSlab(const StackSlab& memoryPool) {(void)memoryPool;};
		StackSlab& operator=(const StackSlab& assign) {(void)assign; return (*this);};
};

#endif // MEMORY_BLOCK_TCC
