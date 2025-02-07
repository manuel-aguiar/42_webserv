


// Project headers
# include "ThreadPoolImpl.hpp"
# include "../TaskQueue/TaskQueue.hpp"
# include "../ThreadWorker/ThreadWorker.hpp"
# include "../../Assert/AssertEqual/AssertEqual.h"

// C headers
# include <pthread.h>

ThreadPoolImpl::ThreadPoolImpl() :
	m_threads(NULL),
	m_exitingThreads(NULL)
{

}

ThreadPoolImpl::~ThreadPoolImpl()
{
	destroy(false);
	pthread_mutex_destroy(&m_statusLock);
	pthread_cond_destroy(&m_exitSignal);
}

void	ThreadPoolImpl::init(ObjectPoolImpl<ThreadWorker>& threads, 
							FixedCircularQueueImpl<ThreadWorker*>& exitingThreads,
							FixedCircularQueueImpl<IThreadTask*>& taskQueue,
							const size_t InitialThreads)
{
	m_threads = &threads;
	m_exitingThreads = &exitingThreads;
	m_taskQueue.init(taskQueue);

	ThreadWorker* worker;

	pthread_mutex_init(&m_statusLock, NULL);
	pthread_cond_init(&m_exitSignal, NULL);

	pthread_mutex_lock(&m_statusLock);
	
	for (unsigned int i = 0; i < InitialThreads; ++i)
	{
		worker = m_threads->allocate();
		new (worker) ThreadWorker(*this);
		worker->start();
	}
	pthread_mutex_unlock(&m_statusLock);
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
	
	for (size_t i = 0; i < m_threads->getElemCount(); ++i)
		m_taskQueue.addTask(NULL);

	pthread_mutex_lock(&m_statusLock);
	while (m_threads->getElemCount())
	{
		mf_destroyExitingThreads();
		if (m_threads->getElemCount())
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

	ASSERT_EQUAL(m_threads->getElemCount() < m_threads->capacity(), true, "ThreadPoolImpl: No more threads can be added");

	pthread_mutex_lock(&m_statusLock);

	worker = m_threads->allocate();
	new (worker) ThreadWorker(*this);
	worker->start();
	
	pthread_mutex_unlock(&m_statusLock);
}

void	ThreadPoolImpl::removeThread()
{
	ASSERT_EQUAL(m_threads->getElemCount() > 0, true, "ThreadPoolImpl: No threads to remove");
	
	pthread_mutex_lock(&m_statusLock);
	m_taskQueue.addTask(NULL);
	pthread_cond_wait(&m_exitSignal, &m_statusLock);
	
	mf_destroyExitingThreads();
	
	pthread_mutex_unlock(&m_statusLock);
}

size_t	ThreadPoolImpl::getThreadCount() const
{
	return (m_threads->getElemCount());
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
	m_exitingThreads->push_back(&worker);
}

void	ThreadPoolImpl::mf_destroyExitingThreads()
{
	size_t total = m_exitingThreads->size();
	
	for (size_t i = 0; i < total; ++i)
	{
		m_exitingThreads->front()->finish();
		m_threads->destroy(m_exitingThreads->front());
		m_threads->deallocate(m_exitingThreads->front());
		m_exitingThreads->pop_front();
	}
}

IThreadTask*	ThreadPoolImpl::mf_acquireTask()
{
	return (m_taskQueue.acquireTask());
}

void	ThreadPoolImpl::mf_finishTask(IThreadTask* delTask)
{
	m_taskQueue.finishTask(delTask);
}

pthread_mutex_t&	ThreadPoolImpl::mf_accessStatusLock()
{
	return (m_statusLock);
}

pthread_cond_t&	ThreadPoolImpl::mf_accessExitSignal()
{
	return (m_exitSignal);
}

// private copy and assignment

ThreadPoolImpl::ThreadPoolImpl(const ThreadPoolImpl& copy)
{
	ASSERT_EQUAL(false, true, "ThreadPoolImpl: Cannot copy");
	(void)copy;
}

ThreadPoolImpl& ThreadPoolImpl::operator=(const ThreadPoolImpl& assign)
{
	ASSERT_EQUAL(false, true, "ThreadPoolImpl: Cannot assign");
	(void)assign;
	return (*this);
}

/////////////////////////////////////////////////////

ThreadPoolGeneric::ThreadPoolGeneric() : ThreadPoolImpl() {}
ThreadPoolGeneric::~ThreadPoolGeneric() {}
ThreadPoolGeneric::ThreadPoolGeneric(const ThreadPoolGeneric& copy) : ThreadPoolImpl(copy) {}
ThreadPoolGeneric& ThreadPoolGeneric::operator=(const ThreadPoolGeneric& assign)
{
	if (this == &assign)
		return (*this);
	ThreadPoolImpl::operator=(assign);
	return (*this);
}

void ThreadPoolGeneric::waitForCompletion() {ThreadPoolImpl::waitForCompletion();}
void ThreadPoolGeneric::destroy(bool waitForCompletion) {ThreadPoolImpl::destroy(waitForCompletion);}
void ThreadPoolGeneric::forceDestroy() {ThreadPoolImpl::forceDestroy();}
void ThreadPoolGeneric::addThread() {ThreadPoolImpl::addThread();}
void ThreadPoolGeneric::removeThread() {ThreadPoolImpl::removeThread();}
bool ThreadPoolGeneric::addTask(IThreadTask& newTask, bool waitForSlot) {return ThreadPoolImpl::addTask(newTask, waitForSlot);}
size_t ThreadPoolGeneric::getThreadCount() const {return ThreadPoolImpl::getThreadCount();}
size_t ThreadPoolGeneric::getTaskCount() {return ThreadPoolImpl::getTaskCount();}

void ThreadPoolGeneric::init(ObjectPoolImpl<ThreadWorker>& threads, 
                              FixedCircularQueueImpl<ThreadWorker*>& exitingThreads,
                              FixedCircularQueueImpl<IThreadTask*>& taskQueue,
                              const size_t InitialThreads) 
{ThreadPoolImpl::init(threads, exitingThreads, taskQueue, InitialThreads);
}