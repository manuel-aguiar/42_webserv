

#ifndef Heap_MemoryPool_HPP

# define Heap_MemoryPool_HPP

# include <memory>

typedef unsigned char t_byte;

template <typename T = t_byte, typename Allocator = std::allocator<T> >
class Impl_Heap_MemoryPool;

typedef Impl_Heap_MemoryPool<> Heap_MemoryPool;

# include "Heap_MemoryPool.tpp"

#endif