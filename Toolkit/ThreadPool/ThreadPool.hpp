

#ifndef THREADPOOL_HPP

# define THREADPOOL_HPP

# include <cstddef>

template <size_t ThreadBacklog, size_t TaskBacklog>
class ThreadPool;

class ThreadPoolHeap;

# include "ThreadPool/ThreadPool.tpp"
# include "ThreadPool/ThreadPoolHeap.tpp"

#endif