

#ifndef NGINX_FIXEDPOOL_HPP

# define NGINX_FIXEDPOOL_HPP

// Project headers
# include "../../Assert/AssertEqual/AssertEqual.h"

// C++ headers
# include <cstddef>

//Base class
# include "../_FixedMemoryPoolImpl/FixedMemoryPoolImpl.hpp"


template <typename T, typename Allocator>
class Heap_MemoryPoolGeneric : public FixedMemoryPoolImpl<T>
{
    public:

        typedef typename Allocator::template rebind<T>::other TypeAllocator;

        Heap_MemoryPoolGeneric(size_t blockSize, const Allocator& allocator = Allocator()) :
            FixedMemoryPoolImpl<T>(NULL, 0),
            m_allocator(allocator),
            m_array(NULL)
        {
            initialize(blockSize);
        } 

        ~Heap_MemoryPoolGeneric()
        {
            if (m_array)
                m_allocator.deallocate(m_array, this->capacity());
        }


        const Allocator& getAllocator() const { return (m_allocator); }

    private:
        TypeAllocator   m_allocator;
        T*              m_array;

        void    initialize(size_t blockSize)
        {
            m_array = m_allocator.allocate(blockSize);
            FixedMemoryPoolImpl<T>::reset(m_array, blockSize);
        }

        Heap_MemoryPoolGeneric(const Heap_MemoryPoolGeneric& copy) {(void)copy;}
        Heap_MemoryPoolGeneric& operator=(const Heap_MemoryPoolGeneric& assign) {(void)assign; return (*this);}

};


#endif
