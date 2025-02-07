

#ifndef THREADPOOL_TPP

# define THREADPOOL_TPP

// Project headers
# include "../../MemoryPool/Stack_ObjectPool/Stack_ObjectPool.hpp"
# include "../../Arrays/StackCircularQueue/StackCircularQueue.hpp"

// ThreadPool headers
# include "../ThreadTask/IThreadTask.hpp"

//Base class
# include "../ThreadPoolImpl/ThreadPoolImpl.hpp"

template <size_t ThreadBacklog, size_t TaskBacklog>
class ThreadPool : public ThreadPoolGeneric
{
	public:

		ThreadPool(size_t InitialThreads = ThreadBacklog) :
			ThreadPoolGeneric()
		{ThreadPoolGeneric::init(m_threads, m_exitingThreads, m_taskQueue, InitialThreads);};

		~ThreadPool() {};

	private:

		pthread_mutex_t									m_statusLock;
		pthread_cond_t									m_exitSignal;

		Stack_ObjectPool<ThreadWorker, ThreadBacklog>	m_threads;
		StackCircularQueue<ThreadWorker*, ThreadBacklog>m_exitingThreads;		
		StackCircularQueue<IThreadTask*, TaskBacklog>	m_taskQueue;

		// Private copy and assignment
		ThreadPool(const ThreadPool& copy) {(void)copy;}
		ThreadPool& operator=(const ThreadPool& assign) {(void)assign; return (*this);}

};


#endif