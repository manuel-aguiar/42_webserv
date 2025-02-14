

#ifndef THREADPOOL_TPP

# define THREADPOOL_TPP

// Project headers
# include "../../MemoryPool/StackSlab/StackSlab.hpp"
# include "../../Arrays/StackCircularQueue/StackCircularQueue.hpp"

// ThreadPool headers
# include "../ThreadTask/IThreadTask.hpp"

//Base class
# include "../_ThreadPoolImpl/ThreadPoolImpl.hpp"

template <size_t ThreadBacklog, size_t TaskBacklog>
class ThreadPool : public ThreadPoolGeneric
{
	public:

		ThreadPool(size_t InitialThreads = ThreadBacklog) :
			ThreadPoolGeneric()
		{ThreadPoolGeneric::init(m_threads, m_exitingThreads, m_taskQueue, InitialThreads);};

		~ThreadPool() {};

	private:
		StackSlab<sizeof(ThreadWorker), ThreadBacklog>	m_threads;
		StackCircularQueue<ThreadWorker*, ThreadBacklog>m_exitingThreads;		
		StackCircularQueue<IThreadTask*, TaskBacklog>	m_taskQueue;

		// Private copy and assignment
		ThreadPool(const ThreadPool& copy) {(void)copy;}
		ThreadPool& operator=(const ThreadPool& assign) {(void)assign; return (*this);}

};


#endif