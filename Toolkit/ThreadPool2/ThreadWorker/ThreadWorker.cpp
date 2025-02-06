

#include "ThreadWorker.hpp"



#ifndef IMPL_THREADWORKER_TPP

# define IMPL_THREADWORKER_TPP

// Project headers
# include "../ThreadPoolImpl/ThreadPoolImpl.hpp"
# include "../ThreadTask/IThreadTask.hpp"
# include "../TaskQueue/TaskQueue.hpp"

// C++ headers
# include <iostream>

ThreadWorker::ThreadWorker(ThreadPool& pool) :
	m_state(EThread_Unitialized),
	m_thread(0),
	m_pool(pool)
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "ThreadWorker Constructor Called" << std::endl;
	#endif
}

ThreadWorker::~ThreadWorker()
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "ThreadWorker Destructor Called" << std::endl;
	#endif	
}

void	ThreadWorker::run()
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

pthread_t	ThreadWorker::getThreadID() const
{
	return (m_thread);
}

void	ThreadWorker::start()
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

void	ThreadWorker::finish()
{
	if (m_state == EThread_Joined)
		return ;
	pthread_join(m_thread, NULL);
	m_state = EThread_Joined;
}

void*	ThreadWorker::ThreadFunction(void* args)
{
	ThreadWorker* thread = (ThreadWorker*)args;
	thread->run();
	return (NULL);
}

ThreadWorker(const ThreadWorker& copy) : 
	m_state(copy.m_state),
	m_thread(copy.m_thread),
	m_pool(copy.m_pool) {}
	
ThreadWorker& 	
ThreadWorker::operator=(const ThreadWorker& assign)
{
	if (this == &assign)
		return (*this);
	
	m_state = assign.m_state;
	m_thread = assign.m_thread;
	m_pool = assign.m_pool;

	return (*this);
}

#endif
