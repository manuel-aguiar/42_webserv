/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 11:15:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/26 18:14:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "ThreadPool.hpp"
# include "../ThreadWorker/ThreadWorker.hpp"
# include "../TaskQueue/TaskQueue.hpp"

// C headers
# include <pthread.h>

ThreadPool::ThreadPool(size_t InitialThreads, size_t maxThreads) :
	m_maxThreads(maxThreads),
	m_threads(MPool_FixedElem<ThreadWorker>(m_maxThreads))
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

void	ThreadPool::forceDestroy()
{
	m_taskQueue.clear();
	
	pthread_mutex_lock(&m_statusLock);
	
	for (List<ThreadWorker, MPool_FixedElem<ThreadWorker> >::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
		pthread_kill(it->getThreadID(), SIGKILL);

	pthread_mutex_unlock(&m_statusLock);
}



void	ThreadPool::addThread()
{
	assert(m_threads.size() < m_maxThreads);

	pthread_mutex_lock(&m_statusLock);

	m_threads.emplace_back(*this);
	m_threads.back().setLocation(--m_threads.end());
	m_threads.back().start();
	
	pthread_mutex_unlock(&m_statusLock);
}

void	ThreadPool::removeThread()
{
	assert(m_threads.size() > 0);
	
	pthread_mutex_lock(&m_statusLock);
	m_taskQueue.addTask(NULL);
	pthread_cond_wait(&m_exitSignal, &m_statusLock);
	
	m_threads.back().finish();
	m_threads.pop_back();
	
	pthread_mutex_unlock(&m_statusLock);
}

size_t	ThreadPool::getThreadCount() const
{
	return (m_threads.size());
}

size_t	ThreadPool::getTaskCount()
{
	return (m_taskQueue.getTaskCount());
}

void ThreadPool::addTask(IThreadTask& newTask)
{
	m_taskQueue.addTask(&newTask);
}

void	ThreadPool::mf_InternalRemoveThread(ThreadWorker& worker)
{
	m_threads.splice(m_threads.end(), m_threads, worker.getLocation());
}