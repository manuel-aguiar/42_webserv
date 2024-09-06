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
	_queue(queue),
	_curTask(NULL),
	_statusLock(statusLock),
	_exitSignal(exitSignal)
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
	pthread_mutex_lock(&_statusLock);
	_exited = false;
	pthread_mutex_unlock(&_statusLock);

	while ((_curTask = _queue.getTask()))
	{
		_curTask->execute();
		_queue.finishTask(_curTask);
	}

	pthread_mutex_lock(&_statusLock);
	_exited = true;
	pthread_cond_signal(&_exitSignal);
	pthread_mutex_unlock(&_statusLock);

}

bool	ThreadPoolWorker::exitedQueue()
{
	return (_exited);
}

ThreadPoolWorker::ThreadPoolWorker(const ThreadPoolWorker& copy) : 
	AThread(copy),
	_queue(copy._queue),
	_curTask(NULL),
	_statusLock(copy._statusLock),
	_exitSignal(copy._exitSignal),
	_exited(false) {}
	
ThreadPoolWorker& ThreadPoolWorker::operator=(const ThreadPoolWorker& assign)  {(void)assign; return (*this);}


