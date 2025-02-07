

#ifndef THREADPOOLIMPL_HPP

# define THREADPOOLIMPL_HPP

# include "../ThreadWorker/ThreadWorker.hpp"
# include "../TaskQueue/TaskQueue.hpp"

// C headers
# include <signal.h>
# include <pthread.h>

// forward declarations
class IThreadTask;

template <typename T>
class ObjectPoolImpl;

class SlabImpl;

template <typename T>
class FixedCircularQueueImpl;

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
		IThreadTask*								mf_acquireTask();
		void										mf_finishTask(IThreadTask* delTask);
		pthread_mutex_t&							mf_accessStatusLock();
		pthread_cond_t&								mf_accessExitSignal();


	protected:

		//////////////// ThreadPoolImpl ///////////////////

		ThreadPoolImpl();
		~ThreadPoolImpl();
		ThreadPoolImpl(const ThreadPoolImpl& copy);
		ThreadPoolImpl& operator=(const ThreadPoolImpl& assign);

		void init(SlabImpl& 							threads, 
				FixedCircularQueueImpl<ThreadWorker*>& 	exitingThreads,
				FixedCircularQueueImpl<IThreadTask*>& 	taskQueue,
				const size_t 							InitialThreads);
		

	private:


		pthread_mutex_t								m_statusLock;
		pthread_cond_t								m_exitSignal;

		TaskQueue									m_taskQueue;
		SlabImpl*									m_threads;
		FixedCircularQueueImpl<ThreadWorker*>*		m_exitingThreads;		

		void										mf_destroyExitingThreads();



};

/////////////////////////////////////////////////////////////////////////////

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

		void	init(SlabImpl&					 		threads, 
				FixedCircularQueueImpl<ThreadWorker*>& 	exitingThreads,
				FixedCircularQueueImpl<IThreadTask*>& 	taskQueue,
				const size_t 							InitialThreads);

	private:

		// Private copy and assignment
		ThreadPoolGeneric(const ThreadPoolGeneric& copy);
		ThreadPoolGeneric& operator=(const ThreadPoolGeneric& assign);

};

#endif