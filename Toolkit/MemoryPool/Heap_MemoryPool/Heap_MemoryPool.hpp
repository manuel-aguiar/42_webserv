

#ifndef Heap_MemoryPool_HPP

# define Heap_MemoryPool_HPP

# include <memory>

typedef unsigned char t_byte;

template <typename T>
class FixedMemoryPoolImpl;

template <typename T = t_byte, typename Allocator = std::allocator<T> >
class Heap_MemoryPoolGeneric;

typedef Heap_MemoryPoolGeneric<> Heap_MemoryPool;



# include "Heap_MemoryPool.tpp"

#endif
