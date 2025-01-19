

#ifndef Heap_ObjectPool_HPP

# define Heap_ObjectPool_HPP

// C++ headers
# include <limits.h>
# include <stddef.h>
# include <cstring>
# include <iostream>


// Project headers
template <typename T, typename MemoryPool>
class SlabAllocator;

# include "SlabAllocator.tpp"

#endif