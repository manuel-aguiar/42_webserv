

#ifndef HEAPSLAB_HPP

# define HEAPSLAB_HPP

# include <cstddef>
# include <memory>

template <size_t nodeSize, typename Allocator = std::allocator<unsigned char> >
class HeapSlab;

# include "HeapSlab.tpp"

#endif