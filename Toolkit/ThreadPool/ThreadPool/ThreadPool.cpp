/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 11:15:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/26 11:54:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../ThreadWorker/ThreadWorker.hpp"
# include "ThreadPool.hpp"
# include "../TaskQueue/TaskQueue.hpp"

// C headers
# include <pthread.h>

ThreadPool::ThreadPool(unsigned int InitialNumberOfThreads)
{
	pthread_mutex_init(&m_statusLock, NULL);
	pthread_cond_init(&m_exitSignal, NULL);

	pthread_mutex_lock(&m_statusLock);

	m_threads.reserve(InitialNumberOfThreads);

	for (unsigned int i = 0; i < InitialNumberOfThreads; ++i)
	{
		m_threads.emplace_back(m_taskQueue, m_statusLock, m_exitSignal);
		m_threads[i].start();
	}
	pthread_mutex_unlock(&m_statusLock);
}

ThreadPool::~ThreadPool()
{
	destroy(false);
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
		m_threads[i].join();
		m_threads.pop_back();
	}
}

void	ThreadPool::addThread()
{

	m_threads.emplace_back(m_taskQueue, m_statusLock, m_exitSignal);
	m_threads[m_threads.size() - 1].start();
}

void	ThreadPool::removeThread()
{
	pthread_mutex_lock(&m_statusLock);
	m_taskQueue.addTask(NULL);
	pthread_cond_wait(&m_exitSignal, &m_statusLock);

		
	for (DynArray<ThreadWorker>::iterator curThread = m_threads.begin(); curThread != m_threads.end(); ++curThread)
	{
		if ((*curThread).exitedQueue())
		{
			(*curThread).join();
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