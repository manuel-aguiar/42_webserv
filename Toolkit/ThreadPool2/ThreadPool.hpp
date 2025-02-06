

#ifndef THREADPOOL_HPP

# define THREADPOOL_HPP

# include <cstddef>

template <size_t ThreadBacklog, size_t TaskBacklog>
class ThreadPool;

# include "ThreadPool/ThreadPool.tpp"

#endif