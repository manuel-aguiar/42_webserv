/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadWorker.cpp							   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: manuel <manuel@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/27 07:56:13 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/29 08:31:38 by manuel		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef IMPL_THREADWORKER_TPP

# define IMPL_THREADWORKER_TPP

// Project headers
# include "../ThreadPool/ThreadPool.hpp"
# include "../ThreadTask/IThreadTask.hpp"
# include "Impl_TaskQueue.tpp"

// C++ headers
# include <iostream>

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker::ThreadWorker(ThreadPool& pool) :
	m_state(EThread_Unitialized),
	m_thread(0),
	m_pool(pool)
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "ThreadWorker Constructor Called" << std::endl;
	#endif
}

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker::~ThreadWorker()
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "ThreadWorker Destructor Called" << std::endl;
	#endif	
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker::run()
{   
	IThreadTask* curTask = NULL;

	while ((curTask = m_pool.m_taskQueue.acquireTask()))
	{
		curTask->execute();
		m_pool.m_taskQueue.finishTask(curTask);
	}
	pthread_mutex_lock(&m_pool.m_statusLock);

	m_pool.mf_markExitingThread(*this);

	pthread_cond_signal(&m_pool.m_exitSignal);
	pthread_mutex_unlock(&m_pool.m_statusLock);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
pthread_t	ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker::getThreadID() const
{
	return (m_thread);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker::start()
{
	if (m_state == EThread_Initialized)
	{
		std::cerr << "Thread already initialized" << std::endl;
		return ;
	}
	if (pthread_create(&m_thread, NULL, ThreadFunction, this))
	{
		std::cerr << "Thread failed to create" << std::endl;
		return ;
	}
	m_state = EThread_Initialized;
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker::finish()
{
	if (m_state == EThread_Joined)
		return ;
	pthread_join(m_thread, NULL);
	m_state = EThread_Joined;
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void*	ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker::ThreadFunction(void* args)
{
	ThreadWorker* thread = (ThreadWorker*)args;
	thread->run();
	return (NULL);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker::ThreadWorker(const ThreadWorker& copy) : 
	m_state(copy.m_state),
	m_thread(copy.m_thread),
	m_pool(copy.m_pool) {}
	
template <size_t ThreadBacklog, size_t TaskBacklog>
typename ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker& 	
ThreadPool<ThreadBacklog, TaskBacklog>::ThreadWorker::operator=(const ThreadWorker& assign)
{
	if (this == &assign)
		return (*this);
	
	m_state = assign.m_state;
	m_thread = assign.m_thread;
	m_pool = assign.m_pool;

	return (*this);
}

#endif
