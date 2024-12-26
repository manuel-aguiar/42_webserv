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

// Project headers
# include "ThreadWorker.hpp"
# include "../ThreadTask/IThreadTask.hpp"
# include "../TaskQueue/TaskQueue.hpp"

// C++ headers
# include <iostream>

ThreadPool::ThreadWorker::ThreadWorker(ThreadPool& pool) :
	m_state(EThread_Unitialized),
	m_thread(0),
	m_pool(pool)
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "ThreadWorker Constructor Called" << std::endl;
	#endif
}

ThreadPool::ThreadWorker::~ThreadWorker()
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "ThreadWorker Destructor Called" << std::endl;
	#endif	
}

void	ThreadPool::ThreadWorker::run()
{   
	IThreadTask* curTask = NULL;

	while ((curTask = m_pool.m_taskQueue.getTask()))
	{
		curTask->execute();
		m_pool.m_taskQueue.finishTask(curTask);
	}
	pthread_mutex_lock(&m_pool.m_statusLock);

	m_pool.mf_InternalRemoveThread(*this);

	pthread_cond_signal(&m_pool.m_exitSignal);
	pthread_mutex_unlock(&m_pool.m_statusLock);
}

void	ThreadPool::ThreadWorker::setLocation(List<ThreadWorker, MPool_FixedElem<ThreadWorker> >::iterator location)
{
	m_location = location;
}

const List<ThreadPool::ThreadWorker, MPool_FixedElem<ThreadPool::ThreadWorker> >::iterator&
				ThreadPool::ThreadWorker::getLocation() const
{
	return (m_location);
}

pthread_t ThreadPool::ThreadWorker::getThreadID() const
{
	return (m_thread);
}

void	ThreadPool::ThreadWorker::start()
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

void	ThreadPool::ThreadWorker::finish()
{
	if (m_state == EThread_Joined)
		return ;
	pthread_join(m_thread, NULL);
	m_state = EThread_Joined;
}

void*   ThreadPool::ThreadWorker::ThreadFunction(void* args)
{
	ThreadWorker* thread = (ThreadWorker*)args;
	thread->run();
	return (NULL);
}

ThreadPool::ThreadWorker::ThreadWorker(const ThreadWorker& copy) : 
	m_state(copy.m_state),
	m_thread(copy.m_thread),
	m_pool(copy.m_pool) {}
	
ThreadPool::ThreadWorker& ThreadPool::ThreadWorker::operator=(const ThreadWorker& assign)  {(void)assign; return (*this);}


