/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadPool.hpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/27 09:30:02 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/30 08:58:09 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef THREADPOOL_TPP

# define THREADPOOL_TPP

// Project headers
# include "StackSlotArray/StackSlotArray.hpp"
# include "../../Arrays/StackCircularQueue/StackCircularQueue.hpp"
# include "../../List/List.hpp"

// ThreadPool headers
# include "../ThreadTask/IThreadTask.hpp"

// C headers
# include <signal.h>

template <size_t ThreadBacklog, size_t TaskBacklog>
class ThreadPool
{
	public:

		/************** ThreadPool *************** */

		ThreadPool(size_t InitialThreads);
		~ThreadPool();

		void	waitForCompletion();
		void	destroy(bool waitForCompletion);
		void	forceDestroy();

		void	addThread();
		void	removeThread();
		
		bool	addTask(IThreadTask& newTask, bool waitForSlot = false);
		bool	addTask(const IThreadTask& newTask, bool waitForSlot = false);

		size_t	getThreadCount() const;
		size_t	getTaskCount();

	private:

		/************** ThreadWorker *************** */

		class ThreadWorker
		{
			public:
				ThreadWorker(ThreadPool& pool);
				~ThreadWorker();
				ThreadWorker(const ThreadWorker& copy);
				ThreadWorker& operator=(const ThreadWorker& assign);

				void		start();
				void		finish();

				pthread_t	getThreadID() const;

			private:
				void	run();

				static void*   ThreadFunction(void* args);

				enum EThreadState
				{
					EThread_Unitialized,
					EThread_Initialized,
					EThread_Joinable,
					EThread_Joined
				};

				EThreadState			m_state;
				pthread_t	   			m_thread;

				ThreadPool&				m_pool;

		};

		/***************Task Queue****************** */

		class TaskQueue
		{
			public:
				TaskQueue();
				~TaskQueue();

				bool				addTask(IThreadTask* newTask, bool waitForSlot = false);
				void				clear();
				void				waitForCompletion();		
				void				finishTask(IThreadTask* delTask);

				IThreadTask*		acquireTask();

				size_t			 	getTaskCount();

			private:
				StackCircularQueue<IThreadTask*, TaskBacklog>	m_tasks;
				unsigned int									m_tasksExecuting;
				pthread_mutex_t									m_taskAccess;
				pthread_cond_t									m_newTaskSignal;								   
				pthread_cond_t									m_allTasksDone;
				pthread_cond_t									m_fullQueue;									 

				TaskQueue(const TaskQueue& copy);
				TaskQueue& operator=(const TaskQueue& assign);
		};

		void										mf_markExitingThread(ThreadWorker& worker);
		void										mf_destroyExitingThreads();

		TaskQueue									m_taskQueue;
		StackSlotArray<ThreadWorker, ThreadBacklog>
													m_threads;
		pthread_mutex_t								m_statusLock;
		pthread_cond_t								m_exitSignal;

		StackCircularQueue<ThreadWorker*, ThreadBacklog>
													m_exitingThreads;		


		// Private copy and assignment
		ThreadPool(const ThreadPool& copy);
		ThreadPool& operator=(const ThreadPool& assign);

};

# include "Impl_TaskQueue.tpp"
# include "Impl_ThreadWorker.tpp"
# include "Impl_ThreadPool.tpp"


#endif