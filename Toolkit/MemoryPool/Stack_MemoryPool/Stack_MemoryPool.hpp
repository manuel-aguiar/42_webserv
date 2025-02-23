

#ifndef Impl_Stack_MemoryPool_HPP

# define Impl_Stack_MemoryPool_HPP

# include <cstddef>

typedef unsigned char t_byte;

template <typename T = t_byte>
class FixedMemoryPoolImpl;

template <size_t BlockSize, typename T = t_byte>
class Stack_MemoryPoolGeneric;

template <size_t BlockSize>
class Stack_MemoryPool : public Stack_MemoryPoolGeneric<BlockSize, t_byte> {};

# include "Stack_MemoryPool.tpp"

#endif
