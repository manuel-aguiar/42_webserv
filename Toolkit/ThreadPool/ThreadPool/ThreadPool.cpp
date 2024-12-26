/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 11:15:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/26 15:40:07 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "../ThreadWorker/ThreadWorker.hpp"
# include "ThreadPool.hpp"
# include "../TaskQueue/TaskQueue.hpp"

// C headers
# include <pthread.h>

ThreadPool::ThreadPool(size_t InitialThreads, size_t maxThreads) :
	m_threads(MPool_FixedElem<ThreadWorker>(maxThreads))
{
	pthread_mutex_init(&m_statusLock, NULL);
	pthread_cond_init(&m_exitSignal, NULL);

	pthread_mutex_lock(&m_statusLock);

	for (unsigned int i = 0; i < InitialThreads; ++i)
	{
		m_threads.emplace_back(*this, m_taskQueue, m_statusLock, m_exitSignal);
		m_threads.back().setLocation(--m_threads.end());
		m_threads.back().start();
	}
	pthread_mutex_unlock(&m_statusLock);
}

ThreadPool::~ThreadPool()
{
	destroy(false);
	pthread_mutex_destroy(&m_statusLock);
	pthread_cond_destroy(&m_exitSignal);
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

void	ThreadPool::addThread()
{
	m_threads.emplace_back(*this, m_taskQueue, m_statusLock, m_exitSignal);
	m_threads.back().setLocation(--m_threads.end());
	m_threads.back().start();
}

void	ThreadPool::removeThread()
{
	pthread_mutex_lock(&m_statusLock);
	m_taskQueue.addTask(NULL);
	pthread_cond_wait(&m_exitSignal, &m_statusLock);
	
	m_threads.back().finish();
	m_threads.pop_back();
	
	pthread_mutex_unlock(&m_statusLock);
}

int	ThreadPool::threadCount() const
{
	return (m_threads.size());
}

void	ThreadPool::mf_InternalRemoveThread(ThreadWorker& worker)
{
	m_threads.splice(m_threads.end(), m_threads, worker.getLocation());
}