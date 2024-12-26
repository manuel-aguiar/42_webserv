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

ThreadWorker::ThreadWorker(TaskQueue& queue, pthread_mutex_t& statusLock, pthread_cond_t& exitSignal) :
	m_state(EThread_Unitialized),
	m_thread(0),
	m_queue(queue),
	m_curTask(NULL),
	m_statusLock(statusLock),
	m_exitSignal(exitSignal)
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
	pthread_mutex_lock(&m_statusLock);
	m_exited = false;
	pthread_mutex_unlock(&m_statusLock);

	while ((m_curTask = m_queue.getTask()))
	{
		m_curTask->execute();
		m_queue.finishTask(m_curTask);
	}

	pthread_mutex_lock(&m_statusLock);
	m_exited = true;
	pthread_cond_signal(&m_exitSignal);
	pthread_mutex_unlock(&m_statusLock);

}

bool	ThreadWorker::exitedQueue()
{
	return (m_exited);
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

void	ThreadWorker::join()
{
	pthread_join(m_thread, NULL);
	m_state = EThread_Joined;
}

void*   ThreadWorker::ThreadFunction(void* args)
{
	ThreadWorker* thread = (ThreadWorker*)args;
	thread->run();
	return (NULL);
}

ThreadWorker::ThreadWorker(const ThreadWorker& copy) : 
	m_state(copy.m_state),
	m_thread(copy.m_thread),
	m_queue(copy.m_queue),
	m_curTask(NULL),
	m_statusLock(copy.m_statusLock),
	m_exitSignal(copy.m_exitSignal),
	m_exited(false) {}
	
ThreadWorker& ThreadWorker::operator=(const ThreadWorker& assign)  {(void)assign; return (*this);}


