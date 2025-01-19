
#ifndef HEAPCIRCULARQUEUE_HPP

# define HEAPCIRCULARQUEUE_HPP

#include <memory>

template <typename T, typename Allocator = std::allocator<T> >
class HeapCircularQueue;

# include "HeapCircularQueue.tpp"

#endif