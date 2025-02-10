

#include "ThreadPoolHeap.hpp"

ThreadPoolHeap::ThreadPoolHeap(size_t threadCount, size_t taskBacklog) :
    ThreadPoolGeneric(),
    m_threads(threadCount),
    m_exitingThreads(threadCount),
    m_taskQueue(taskBacklog)

{
    ThreadPoolGeneric::init(m_threads, m_exitingThreads, m_taskQueue, threadCount);
};

ThreadPoolHeap::~ThreadPoolHeap() { destroy(false); };