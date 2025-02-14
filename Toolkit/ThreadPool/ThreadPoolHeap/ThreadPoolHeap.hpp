

#ifndef THREADPOOLHEAP_HPP

# define THREADPOOLHEAP_HPP

// Project headers
# include "../../MemoryPool/HeapSlab/HeapSlab.hpp"
# include "../../Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

// ThreadPool headers
# include "../ThreadTask/IThreadTask.hpp"

//Base class
# include "../_ThreadPoolImpl/ThreadPoolImpl.hpp"

class ThreadPoolHeap : public ThreadPoolGeneric
{
	public:
		ThreadPoolHeap(size_t threadCount, size_t taskBacklog);
		~ThreadPoolHeap();

	private:
		HeapSlab<sizeof(ThreadWorker)>		m_threads;
		HeapCircularQueue<ThreadWorker*>	m_exitingThreads;		
		HeapCircularQueue<IThreadTask*>		m_taskQueue;

};


#endif