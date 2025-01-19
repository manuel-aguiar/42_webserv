

#ifndef SHAREDMPOOL_ALIGNDEALLOC_HPP

# define SHAREDMPOOL_ALIGNDEALLOC_HPP

# include "MemoryPool_AlignDealloc.hpp"

template <typename T, size_t BlockSize = 4096, size_t StartingBlocks = 0, size_t SpareBlocks = 0>
class SharedMPool_AlignDealloc;

# include "SharedMPool_AlignDealloc.tpp"

#endif