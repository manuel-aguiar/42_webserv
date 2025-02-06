

#ifndef THREADPOOLIMPL_HPP

# define THREADPOOLMPL_HPP

// Project headers
# include "../../MemoryPool/Stack_ObjectPool/Stack_ObjectPool.hpp"
# include "../../Arrays/StackCircularQueue/StackCircularQueue.hpp"

# include "../TaskQueue/TaskQueue.hpp"

// C headers
# include <signal.h>

// forward declarations
class ThreadWorker;
class IThreadTask;

class ThreadPoolImpl
{


		void	waitForCompletion();
		void	destroy(bool waitForCompletion);
		void	forceDestroy();

		void	addThread();
		void	removeThread();
		
		bool	addTask(IThreadTask& newTask, bool waitForSlot = false);
		//bool	addTask(const IThreadTask& newTask, bool waitForSlot = false);

		size_t	getThreadCount() const;
		size_t	getTaskCount();

	protected:

		//////////////// ThreadPoolImpl ///////////////////

		ThreadPoolImpl(
            
        );
		~ThreadPoolImpl();

	private:

		void										mf_markExitingThread(ThreadWorker& worker);
		void										mf_destroyExitingThreads();

		TaskQueue									m_taskQueue;
		Stack_ObjectPool<ThreadWorker, ThreadBacklog>
													m_threads;
		pthread_mutex_t								m_statusLock;
		pthread_cond_t								m_exitSignal;

		StackCircularQueue<ThreadWorker*, ThreadBacklog>
													m_exitingThreads;		


		// Private copy and assignment
		ThreadPoolImpl(const ThreadPoolImpl& copy);
		ThreadPoolImpl& operator=(const ThreadPoolImpl& assign);

};


#endif