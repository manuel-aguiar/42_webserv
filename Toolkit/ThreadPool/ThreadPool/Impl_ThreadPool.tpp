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

#ifndef IMPL_THREADPOOL_HPP

# define IMPL_THREADPOOL_HPP

// Project headers
# include "ThreadPool.hpp"

// C headers
# include <pthread.h>

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::ThreadPool(size_t InitialThreads) :
	m_threads(MPool_FixedElem<ThreadWorker>(ThreadBacklog))
{
	pthread_mutex_init(&m_statusLock, NULL);
	pthread_cond_init(&m_exitSignal, NULL);

	pthread_mutex_lock(&m_statusLock);

	for (unsigned int i = 0; i < InitialThreads; ++i)
	{
		m_threads.emplace_back(*this);
		m_threads.back().setLocation(--m_threads.end());
		m_threads.back().start();
	}
	pthread_mutex_unlock(&m_statusLock);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::~ThreadPool()
{
	destroy(false);
	pthread_mutex_destroy(&m_statusLock);
	pthread_cond_destroy(&m_exitSignal);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::waitForCompletion()
{
	m_taskQueue.waitForCompletion();
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::destroy(bool waitForCompletion)
{
	if (!waitForCompletion)
		m_taskQueue.clear();

	m_taskQueue.waitForCompletion();
	
	while (m_threads.size())
	{
		pthread_mutex_lock(&m_statusLock);
		m_taskQueue.addTask(NULL);
		pthread_cond_wait(&m_exitSignal, &m_statusLock);
		m_threads.back().finish();
		m_threads.pop_back();
		pthread_mutex_unlock(&m_statusLock);
	}
}


template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::forceDestroy()
{
	m_taskQueue.clear();
	
	pthread_mutex_lock(&m_statusLock);
	
	for(size_t i = 0; i < m_threads.size(); ++i)
		pthread_kill(m_threads[i].getThreadID(), SIGKILL);

	pthread_mutex_unlock(&m_statusLock);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::addThread()
{
	assert(m_threads.size() < ThreadBacklog);

	pthread_mutex_lock(&m_statusLock);

	m_threads.emplace_back(*this);
	m_threads.back().setLocation(--m_threads.end());
	m_threads.back().start();
	
	pthread_mutex_unlock(&m_statusLock);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::removeThread()
{
	assert(m_threads.size() > 0);
	
	pthread_mutex_lock(&m_statusLock);
	m_taskQueue.addTask(NULL);
	pthread_cond_wait(&m_exitSignal, &m_statusLock);
	
	m_threads.back().finish();
	m_threads.pop_back();
	
	pthread_mutex_unlock(&m_statusLock);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
size_t	ThreadPool<ThreadBacklog, TaskBacklog>::getThreadCount() const
{
	return (m_threads.size());
}

template <size_t ThreadBacklog, size_t TaskBacklog>
size_t	ThreadPool<ThreadBacklog, TaskBacklog>::getTaskCount()
{
	return (m_taskQueue.getTaskCount());
}

template <size_t ThreadBacklog, size_t TaskBacklog>
bool	ThreadPool<ThreadBacklog, TaskBacklog>::addTask(IThreadTask& newTask, bool waitForSlot)
{
	return (m_taskQueue.addTask(&newTask, waitForSlot));
}

template <size_t ThreadBacklog, size_t TaskBacklog>
bool	ThreadPool<ThreadBacklog, TaskBacklog>::addTask(const IThreadTask& newTask, bool waitForSlot)
{
	return (m_taskQueue.addTask(&newTask, waitForSlot));
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::mf_InternalRemoveThread(ThreadWorker& worker)
{
	m_threads.splice(m_threads.end(), m_threads, worker.getLocation());
}


// private copy and assignment

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::ThreadPool(const ThreadPool& copy)
{
	(void)copy;
}

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>& ThreadPool<ThreadBacklog, TaskBacklog>::operator=(const ThreadPool& assign)
{
	(void)assign;
	return (*this);
}

#endif