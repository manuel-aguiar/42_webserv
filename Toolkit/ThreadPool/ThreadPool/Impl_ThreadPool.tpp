

#ifndef IMPL_THREADPOOL_TPP

# define IMPL_THREADPOOL_TPP

// Project headers
# include "../ThreadPool.hpp"

// C headers
# include <pthread.h>

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::ThreadPool(size_t InitialThreads)
{
	ThreadWorker* worker;

	pthread_mutex_init(&m_statusLock, NULL);
	pthread_cond_init(&m_exitSignal, NULL);

	pthread_mutex_lock(&m_statusLock);
	
	for (unsigned int i = 0; i < InitialThreads; ++i)
	{
		worker = m_threads.allocate();
		new (worker) ThreadWorker(*this);
		worker->start();
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
	
	for (size_t i = 0; i < m_threads.getElemCount(); ++i)
		m_taskQueue.addTask(NULL);
	
	pthread_mutex_lock(&m_statusLock);
	while (m_threads.getElemCount())
	{
		pthread_cond_wait(&m_exitSignal, &m_statusLock);
		mf_destroyExitingThreads();		
	}
	pthread_mutex_unlock(&m_statusLock);
}


template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::forceDestroy()
{
	m_taskQueue.clear();
	
	pthread_mutex_lock(&m_statusLock);
	
	for(size_t i = 0; i < m_threads.getElemCount(); ++i)
		pthread_kill(m_threads[i].getThreadID(), SIGKILL);

	pthread_mutex_unlock(&m_statusLock);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::addThread()
{
	ThreadWorker* worker;

	assert(m_threads.getElemCount() < ThreadBacklog);

	pthread_mutex_lock(&m_statusLock);

	worker = m_threads.allocate();
	new (worker) ThreadWorker(*this);
	worker->start();
	
	pthread_mutex_unlock(&m_statusLock);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::removeThread()
{
	assert(m_threads.getElemCount() > 0);
	
	pthread_mutex_lock(&m_statusLock);
	m_taskQueue.addTask(NULL);
	pthread_cond_wait(&m_exitSignal, &m_statusLock);
	
	mf_destroyExitingThreads();
	
	pthread_mutex_unlock(&m_statusLock);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
size_t	ThreadPool<ThreadBacklog, TaskBacklog>::getThreadCount() const
{
	return (m_threads.getElemCount());
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
void	ThreadPool<ThreadBacklog, TaskBacklog>::mf_markExitingThread(ThreadWorker& worker)
{
	m_exitingThreads.push_back(&worker);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::mf_destroyExitingThreads()
{
	size_t total = m_exitingThreads.size();
	
	for (size_t i = 0; i < total; ++i)
	{
		m_exitingThreads.front()->finish();
		m_threads.destroy(m_exitingThreads.front());
		m_threads.deallocate(m_exitingThreads.front());
		m_exitingThreads.pop_front();
	}
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