/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadPool.cpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/27 09:30:00 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/30 09:06:43 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../include/Concrete/ThreadPool.hpp"

ThreadPool::ThreadPool(unsigned int InitialNumberOfThreads) :
	m_threads(InitialNumberOfThreads)
{
	pthread_mutex_init(&m_statusLock, NULL);
	pthread_cond_init(&m_exitSignal, NULL);

	pthread_mutex_lock(&m_statusLock);
	for (unsigned int i = 0; i < InitialNumberOfThreads; ++i)
	{
		m_threads[i] = new ThreadPoolWorker(m_taskQueue, m_statusLock, m_exitSignal);
		m_threads[i]->start();
	}
	pthread_mutex_unlock(&m_statusLock);
}

ThreadPool::~ThreadPool()
{
	destroy(false);
}

void	ThreadPool::addTask(IThreadTask* newTask)
{
	m_taskQueue.addTask(newTask);
}

void	ThreadPool::addTask(IThreadTask& newTask)
{
	m_taskQueue.addTask(&newTask);
}

void	ThreadPool::waitForCompletion()
{
	m_taskQueue.waitForCompletion();
}

void	ThreadPool::destroy(bool waitForCompletion)
{
	if (!waitForCompletion)
		m_taskQueue.clear();
	m_taskQueue.waitForCompletion();
	for (unsigned int i = 0; i < m_threads.size(); ++i)
		m_taskQueue.addTask(NULL);
	for (int i = m_threads.size() - 1; i >= 0; --i)
	{
		m_threads[i]->join();
		delete (m_threads[i]);
		m_threads.pop_back();
	}
}

void	ThreadPool::addThread()
{
	ThreadPoolWorker*   newThread;

	newThread = new ThreadPoolWorker(m_taskQueue, m_statusLock, m_exitSignal);
	m_threads.push_back(newThread);
	newThread->start();
}

void	ThreadPool::removeThread()
{
	pthread_mutex_lock(&m_statusLock);
	m_taskQueue.addTask(NULL);
	pthread_cond_wait(&m_exitSignal, &m_statusLock);

	std::vector<ThreadPoolWorker*>::iterator curThread;	
	for (curThread = m_threads.begin(); curThread != m_threads.end(); ++curThread)
	{
		if ((*curThread)->exitedQueue())
		{
			(*curThread)->join();
			delete (*curThread);
			m_threads.erase(curThread);
			break ;
		}
	}
	pthread_mutex_unlock(&m_statusLock);
}

int	ThreadPool::threadCount() const
{
	return (m_threads.size());
}