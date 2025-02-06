
#ifndef STACKARRAY_HPP

# define STACKARRAY_HPP

# include <cstddef>

template <typename T>
class StackArrayImpl;

template <typename T, size_t ElemCount>
class StackArray;

# include "StackArray.tpp"

#endif