

#ifndef MEMORY_POOL_ALIGNDEALLOC_HPP

# define MEMORY_POOL_ALIGNDEALLOC_HPP

#include <limits.h>
#include <stddef.h>
#include <cstring>
#include <iostream>

template <typename T, size_t BlockSize = 4096, size_t StartingBlocks = 0, size_t SpareBlocks = 0>
class MemoryPool_AlignDealloc;

# include "MemoryPool_AlignDealloc.tpp"

#endif