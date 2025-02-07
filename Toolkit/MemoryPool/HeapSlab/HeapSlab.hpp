

#ifndef HEAPSLAB_HPP

# define HEAPSLAB_HPP

# include <memory>

template <size_t nodeSize, typename Allocator = std::allocator<char> >
class HeapSlab;

# include "HeapSlab.tpp"

#endif