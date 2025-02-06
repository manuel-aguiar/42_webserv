

#ifndef HEAPSLAB_HPP

# define HEAPSLAB_HPP

class StackSlabImpl;

template <size_t nodeSize, size_t Capacity>
class StackSlab;

# include "StackSlab.tpp"

#endif