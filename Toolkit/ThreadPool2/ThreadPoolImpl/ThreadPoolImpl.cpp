


// Project headers
# include "ThreadPoolImpl.hpp"
# include "../TaskQueue/TaskQueue.hpp"
# include "../ThreadWorker/ThreadWorker.hpp"
# include "../../Assert/AssertEqual/AssertEqual.h"

// C headers
# include <pthread.h>

ThreadPoolImpl::ThreadPoolImpl(size_t InitialThreads)
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

ThreadPoolImpl::~ThreadPoolImpl()
{
	destroy(false);
	pthread_mutex_destroy(&m_statusLock);
	pthread_cond_destroy(&m_exitSignal);
}

void	ThreadPoolImpl::waitForCompletion()
{
	m_taskQueue.waitForCompletion();
}

void	ThreadPoolImpl::destroy(bool waitForCompletion)
{
	if (!waitForCompletion)
		m_taskQueue.clear();

	m_taskQueue.waitForCompletion();
	
	for (size_t i = 0; i < m_threads.getElemCount(); ++i)
		m_taskQueue.addTask(NULL);

	pthread_mutex_lock(&m_statusLock);
	while (m_threads.getElemCount())
	{
		mf_destroyExitingThreads();
		if (m_threads.getElemCount())
			pthread_cond_wait(&m_exitSignal, &m_statusLock);
	}
	pthread_mutex_unlock(&m_statusLock);
}


void	ThreadPoolImpl::forceDestroy()
{
	destroy(false);
}

void	ThreadPoolImpl::addThread()
{
	ThreadWorker* worker;

	ASSERT_EQUAL(m_threads.getElemCount() < ThreadBacklog, true, "ThreadPoolImpl: No more threads can be added");

	pthread_mutex_lock(&m_statusLock);

	worker = m_threads.allocate();
	new (worker) ThreadWorker(*this);
	worker->start();
	
	pthread_mutex_unlock(&m_statusLock);
}

void	ThreadPoolImpl::removeThread()
{
	ASSERT_EQUAL(m_threads.getElemCount() > 0, true, "ThreadPoolImpl: No threads to remove");
	
	pthread_mutex_lock(&m_statusLock);
	m_taskQueue.addTask(NULL);
	pthread_cond_wait(&m_exitSignal, &m_statusLock);
	
	mf_destroyExitingThreads();
	
	pthread_mutex_unlock(&m_statusLock);
}

size_t	ThreadPoolImpl::getThreadCount() const
{
	return (m_threads.getElemCount());
}

size_t	ThreadPoolImpl::getTaskCount()
{
	return (m_taskQueue.getTaskCount());
}

bool	ThreadPoolImpl::addTask(IThreadTask& newTask, bool waitForSlot)
{
	return (m_taskQueue.addTask(&newTask, waitForSlot));
}

//bool	ThreadPoolImpl::addTask(const IThreadTask& newTask, bool waitForSlot)
//{
//	return (m_taskQueue.addTask(&newTask, waitForSlot));
//}

void	ThreadPoolImpl::mf_markExitingThread(ThreadWorker& worker)
{
	m_exitingThreads.push_back(&worker);
}

void	ThreadPoolImpl::mf_destroyExitingThreads()
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

ThreadPoolImpl::ThreadPoolImpl(const ThreadPoolImpl& copy)
{
	(void)copy;
}

ThreadPoolImpl<ThreadBacklog, TaskBacklog>& ThreadPoolImpl::operator=(const ThreadPoolImpl& assign)
{
	(void)assign;
	return (*this);
}