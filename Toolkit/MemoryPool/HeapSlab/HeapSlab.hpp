

#ifndef HEAPSLAB_HPP

# define HEAPSLAB_HPP

# include <memory>
# include <cstddef>

template <size_t nodeSize, typename Allocator = std::allocator<char> >
class HeapSlab;

# include "HeapSlab.tpp"

#endif