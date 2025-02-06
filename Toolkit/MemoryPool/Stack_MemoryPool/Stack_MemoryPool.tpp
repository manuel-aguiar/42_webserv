

#ifndef Stack_MemoryPool_TPP

# define Stack_MemoryPool_TPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstddef>


template <typename T>
class Stack_MemoryPoolImpl
{
    public:
        Stack_MemoryPoolImpl(T* array, size_t BlockSize) :
            m_begin(array),
            m_freePosition(m_begin),
            m_endOfBlock(m_begin + BlockSize) {}

        ~Stack_MemoryPoolImpl() {}

    
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

        size_t                      getBlockSize() const
        {
            return (m_endOfBlock - m_begin);
        }
        size_t                      getFreeSpace() const
        {
            return (m_endOfBlock - m_freePosition);
        }

        int getElementSize() const { return (sizeof(T)); }

    private:
        T*                          m_begin;
        T*                          m_freePosition;
        T*                          m_endOfBlock;

        
        Stack_MemoryPoolImpl(const Stack_MemoryPoolImpl& pool) {(void)pool;}
        Stack_MemoryPoolImpl& operator=(const Stack_MemoryPoolImpl& pool){(void)pool; return (*this);}

        static T*      mf_allignedAlloc(T *location, size_t alignment)
        {
            return ((T *) (((size_t) (location) + ((size_t) alignment - 1)) & ~((size_t) alignment - 1)));
        }
};



template <size_t BlockSize, typename T>
class Stack_MemoryPoolGeneric : public Stack_MemoryPoolImpl<T>
{
    public:
        Stack_MemoryPoolGeneric() : 
            Stack_MemoryPoolImpl<T>(m_array, BlockSize) {}

        ~Stack_MemoryPoolGeneric() {}


    private:
        T                           m_array[BlockSize];

        Stack_MemoryPoolGeneric(const Stack_MemoryPoolGeneric& pool) {(void)pool;}
        Stack_MemoryPoolGeneric& operator=(const Stack_MemoryPoolGeneric& pool){(void)pool; return (*this);}

};


#endif
