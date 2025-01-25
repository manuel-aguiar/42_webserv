

#ifndef Stack_MemoryPool_TPP

# define Stack_MemoryPool_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstddef>


template <size_t BlockSize, typename T>
class Impl_Stack_MemoryPool
{
    public:
        Impl_Stack_MemoryPool() :
            m_freePosition(m_array),
            m_endOfBlock(m_array + BlockSize) {}

        ~Impl_Stack_MemoryPool() {}

    
		T* 						allocate(size_t size)
        {
            return (allocate(size, (size > sizeof(size_t)) ? sizeof(size_t) : size));
        }

        T*                       allocate(size_t size, size_t alignment)
        {
            T* location = mf_allignedAlloc(m_freePosition, alignment);

            ASSERT_EQUAL(location + size <= m_endOfBlock, true, "Stack Memory Pool: Out of memory");

            m_freePosition = location + size;
            return (location);
        }

        void                        reset()
        {
            m_freePosition = m_array;
        }

        size_t                      getBlockSize() const
        {
            return (BlockSize);
        }
        size_t                      getFreeSpace() const
        {
            return (m_endOfBlock - m_freePosition);
        }

        int getElementSize() const { return (sizeof(T)); }

    private:
        T*                          m_freePosition;
        T*                          m_endOfBlock;
        T                           m_array[BlockSize];

        
        Impl_Stack_MemoryPool(const Impl_Stack_MemoryPool& pool) {(void)pool;}
        Impl_Stack_MemoryPool& operator=(const Impl_Stack_MemoryPool& pool){(void)pool; return (*this);}

        static T*      mf_allignedAlloc(T *location, size_t alignment)
        {
            return ((T *) (((size_t) (location) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
        }

};


#endif
