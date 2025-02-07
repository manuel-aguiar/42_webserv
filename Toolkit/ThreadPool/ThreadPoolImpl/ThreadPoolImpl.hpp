

#ifndef THREADPOOLIMPL_HPP

# define THREADPOOLMPL_HPP

// Project headers
# include "../../MemoryPool/Stack_ObjectPool/Stack_ObjectPool.hpp"
# include "../../Arrays/StackCircularQueue/StackCircularQueue.hpp"

# include "../ThreadWorker/ThreadWorker.hpp"
# include "../TaskQueue/TaskQueue.hpp"

// C headers
# include <signal.h>

// forward declarations
class IThreadTask;

class ThreadPoolImpl
{
	public:

		void	waitForCompletion();
		void	destroy(bool waitForCompletion);
		void	forceDestroy();

		void	addThread();
		void	removeThread();
		
		bool	addTask(IThreadTask& newTask, bool waitForSlot = false);
		//bool	addTask(const IThreadTask& newTask, bool waitForSlot = false);

		size_t	getThreadCount() const;
		size_t	getTaskCount();

		void										mf_markExitingThread(ThreadWorker& worker);
		void										mf_destroyExitingThreads();
		IThreadTask*								mf_acquireTask();
		void										mf_finishTask(IThreadTask* delTask);
		pthread_mutex_t&							mf_accessStatusLock();
		pthread_cond_t&								mf_accessExitSignal();


	protected:

		//////////////// ThreadPoolImpl ///////////////////

		ThreadPoolImpl();
		~ThreadPoolImpl();
		// Private copy and assignment
		ThreadPoolImpl(const ThreadPoolImpl& copy);
		ThreadPoolImpl& operator=(const ThreadPoolImpl& assign);

		void init(ObjectPoolImpl<ThreadWorker>& 		threads, 
				FixedCircularQueueImpl<ThreadWorker*>& 	exitingThreads,
				FixedCircularQueueImpl<IThreadTask*>& 	taskQueue,
				const size_t 							InitialThreads);
		

	private:


		pthread_mutex_t								m_statusLock;
		pthread_cond_t								m_exitSignal;

		TaskQueue									m_taskQueue;
		ObjectPoolImpl<ThreadWorker>*				m_threads;
		FixedCircularQueueImpl<ThreadWorker*>*		m_exitingThreads;		




};


class ThreadPoolGeneric : private ThreadPoolImpl
{
	public:

		ThreadPoolGeneric();
		~ThreadPoolGeneric();

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

		void	init(ObjectPoolImpl<ThreadWorker>& 		threads, 
				FixedCircularQueueImpl<ThreadWorker*>& 	exitingThreads,
				FixedCircularQueueImpl<IThreadTask*>& 	taskQueue,
				const size_t 							InitialThreads);

	private:

		// Private copy and assignment
		ThreadPoolGeneric(const ThreadPoolGeneric& copy) : ThreadPoolImpl(copy) {(void)copy;}
		ThreadPoolGeneric& operator=(const ThreadPoolGeneric& assign) {(void)assign; return (*this);}

};

#endif