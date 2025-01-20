

#include "BilateralMap.tpp"
# include "../../MemoryPool/MemoryPool.h"

template <typename T>
class MemoryPool_Wrapper {
public:
    typedef T value_type; // Flags that this allocator will manage

    // Pointer types
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;

    // Size types
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    MemoryPool_Wrapper() : pool() {}

    // Allocate memory for n objects of type T
    pointer allocate(size_type n) {
        return static_cast<pointer>(pool.allocate(n));
    }

    // Deallocate memory for n objects of type T
    void deallocate(pointer p, size_type n) {
        pool.deallocate(p);
    }

    // Other necessary member functions, such as rebind, would go here
    template <typename U>
    struct rebind {
        typedef MemoryPool_Wrapper<U> other;
    };

private:
    MemoryPool_Dealloc<T, 4096, 0, 0> pool; // An instance of the old memory pool
};

int main()
{
    BilateralMap<int, std::string, MemoryPool_Wrapper> map;

    map.insert(1, "cenas");
    map.getValue(1);

    return (0);
}
