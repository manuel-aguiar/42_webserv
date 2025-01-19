

#ifndef Impl_Stack_MemoryPool_HPP

# define Impl_Stack_MemoryPool_HPP

# include <cstddef>

typedef unsigned char t_byte;

template <size_t BlockSize, typename T = t_byte>
class Impl_Stack_MemoryPool;

template <size_t BlockSize>
class Stack_MemoryPool : public Impl_Stack_MemoryPool<BlockSize, t_byte> {};

# include "Stack_MemoryPool.tpp"

#endif