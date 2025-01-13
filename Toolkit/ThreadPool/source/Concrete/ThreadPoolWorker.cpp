/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadPoolWorker.cpp							   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: manuel <manuel@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/27 07:56:13 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/29 08:31:38 by manuel		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

# include "../../include/Concrete/ThreadPoolWorker.hpp"

ThreadPoolWorker::ThreadPoolWorker(IThreadTaskQueue& queue, pthread_mutex_t& statusLock, pthread_cond_t& exitSignal) :
	AThread(),
	m_queue(queue),
	m_curTask(NULL),
	m_statusLock(statusLock),
	m_exitSignal(exitSignal)
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "ThreadPoolWorker Constructor Called" << std::endl;
	#endif
}

ThreadPoolWorker::~ThreadPoolWorker()
{
	#ifdef DEBUG_CONSTRUCTOR
		std::cout << "ThreadPoolWorker Destructor Called" << std::endl;
	#endif	
}

void	ThreadPoolWorker::run()
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

bool	ThreadPoolWorker::exitedQueue()
{
	return (m_exited);
}

ThreadPoolWorker::ThreadPoolWorker(const ThreadPoolWorker& copy) : 
	AThread(copy),
	m_queue(copy.m_queue),
	m_curTask(NULL),
	m_statusLock(copy.m_statusLock),
	m_exitSignal(copy.m_exitSignal),
	m_exited(false) {}
	
ThreadPoolWorker& ThreadPoolWorker::operator=(const ThreadPoolWorker& assign)  {(void)assign; return (*this);}


