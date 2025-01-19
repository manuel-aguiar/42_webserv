



#ifndef Heap_ObjectPool_H

# define Heap_ObjectPool_H

# include <limits.h>
# include <stddef.h>
# include <cstring>
# include <iostream>
# include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class Heap_ObjectPool;

# include "Heap_ObjectPool.tpp"

#endif