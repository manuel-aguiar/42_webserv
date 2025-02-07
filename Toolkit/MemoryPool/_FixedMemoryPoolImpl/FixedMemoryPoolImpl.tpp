


#ifndef FIXEDMEMORYPOOLIMPL_TPP

# define FIXEDMEMORYPOOLIMPL_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstddef>

template <typename T>
class FixedMemoryPoolImpl
{
    public:

		T* 	allocate(size_t size)
        {
            return (allocate(size, (size > sizeof(size_t)) ? sizeof(size_t) : size));
        }

        T*  allocate(size_t size, size_t alignment)
        {
            T* location = mf_allignedAlloc(m_freePosition, alignment);

            ASSERT_EQUAL(location + size <= m_endOfBlock, true, "Stack Memory Pool: Out of memory");

            m_freePosition = location + size;
            return (location);
        }

        void                        reset()
        {
            m_freePosition = m_begin;
        }

        size_t                      capacity() const
        {
            return (m_endOfBlock - m_begin);
        }
        size_t                      getFreeSpace() const
        {
            return (m_endOfBlock - m_freePosition);
        }

        int getElementSize() const { return (sizeof(T)); }

    protected:
        FixedMemoryPoolImpl(T* array, size_t BlockSize) :
            m_begin(array),
            m_freePosition(m_begin),
            m_endOfBlock(m_begin + BlockSize) {}

        ~FixedMemoryPoolImpl() {}
        
        FixedMemoryPoolImpl(const FixedMemoryPoolImpl& pool) :
            m_begin(pool.m_begin),
            m_freePosition(pool.m_freePosition),
            m_endOfBlock(pool.m_endOfBlock) {}

        FixedMemoryPoolImpl& operator=(const FixedMemoryPoolImpl& pool)
        {
            if (this == &pool)
                return (*this);

            m_begin = pool.m_begin;
            m_freePosition = pool.m_freePosition;
            m_endOfBlock = pool.m_endOfBlock;

            return (*this);
        }

        void reset(T* array, size_t BlockSize)
        {
            m_begin = array;
            m_freePosition = m_begin;
            m_endOfBlock = m_begin + BlockSize;
        }

    private:
        T*                          m_begin;
        T*                          m_freePosition;
        T*                          m_endOfBlock;

        

        static T*      mf_allignedAlloc(T *location, size_t alignment)
        {
            return ((T *) (((size_t) (location) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
        }
};


#endif