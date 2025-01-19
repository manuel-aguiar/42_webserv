

#ifndef NGINX_FIXEDPOOL_HPP

# define NGINX_FIXEDPOOL_HPP

// C++ headers
# include <cstddef>
# include <cassert>
# include <iostream>

template <typename T, typename Allocator>
class Impl_Heap_MemoryPool
{
    public:

        typedef typename Allocator::template rebind<T>::other TypeAllocator;

        Impl_Heap_MemoryPool(size_t blockSize, const Allocator& allocator = Allocator()) :
            m_allocator(allocator),
            m_array(m_allocator.allocate(blockSize)), 
            m_freePosition(m_array),
            m_endOfBlock(m_array + blockSize),
            m_blockSize(blockSize) {}

        ~Impl_Heap_MemoryPool() { destroy(); }

        T*  allocate(size_t size) {
            assert(m_array != NULL);
            return (allocate(size, (size * sizeof(T) > sizeof(size_t)) ? sizeof(size_t) : size * sizeof(T)));
        }

        T*  allocate(size_t size, size_t alignment)
        {
            assert(m_array != NULL);

            T* location = mf_allignedAlloc(m_freePosition, alignment);

            assert(location + size <= m_endOfBlock);
            m_freePosition = location + size;
            return (location);    
        }

        int getElementSize() const { return (sizeof(T)); }

        void reset() { m_freePosition = m_array; }

        void destroy()
        {
            m_allocator.deallocate(m_array, m_blockSize);
            m_array = NULL;
        }

        size_t getBlockSize() const { return (m_blockSize); }

        size_t getFreeSpace() const { return (m_endOfBlock - m_freePosition); } 

        const Allocator& getAllocator() const { return (m_allocator); }

    private:
        TypeAllocator   m_allocator;
        T*              m_array;
        T*              m_freePosition;
        T*              m_endOfBlock;
        const size_t    m_blockSize;

        Impl_Heap_MemoryPool() : m_array(NULL), m_blockSize(0) {}
        Impl_Heap_MemoryPool(const Impl_Heap_MemoryPool& copy) : m_blockSize(copy.m_blockSize) {(void)copy;}
        Impl_Heap_MemoryPool& operator=(const Impl_Heap_MemoryPool& assign) {(void)assign; return (*this);}

        static T* mf_allignedAlloc(T* position, size_t alignment) {
            return ((T*)(((size_t)(position) + ((size_t)alignment - 1)) & ~((size_t)alignment - 1)));
        }
};


#endif
