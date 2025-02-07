

#ifndef THREADPOOLHEAP_TPP

# define THREADPOOLHEAP_TPP

// Project headers
# include "../../MemoryPool/Heap_ObjectPool/Heap_ObjectPool.hpp"
# include "../../Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

// ThreadPool headers
# include "../ThreadTask/IThreadTask.hpp"

//Base class
# include "../ThreadPoolImpl/ThreadPoolImpl.hpp"

class ThreadPoolHeap : public ThreadPoolGeneric
{
	public:

		ThreadPoolHeap(size_t threadCount, size_t taskBacklog) :
			ThreadPoolGeneric(),
			m_threads(threadCount),
			m_exitingThreads(threadCount),
			m_taskQueue(taskBacklog)

		{
			ThreadWorker* worker = m_threads.allocate(1);
			m_threads.deallocate(worker);
			ThreadPoolGeneric::init(m_threads, m_exitingThreads, m_taskQueue, threadCount);
		};

		~ThreadPoolHeap() {};

	private:
		Heap_ObjectPool<ThreadWorker>		m_threads;
		HeapCircularQueue<ThreadWorker*>	m_exitingThreads;		
		HeapCircularQueue<IThreadTask*>		m_taskQueue;

};


#endif