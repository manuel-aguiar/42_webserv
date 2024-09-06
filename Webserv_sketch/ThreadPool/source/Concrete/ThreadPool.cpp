/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadPool.cpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com	+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/27 09:30:00 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/30 09:06:43 by mmaria-d		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../include/Concrete/ThreadPool.hpp"

ThreadPool::ThreadPool(unsigned int InitialNumberOfThreads) :
	_threads(InitialNumberOfThreads)
{
	pthread_mutex_init(&_statusLock, NULL);
	pthread_cond_init(&_exitSignal, NULL);

	pthread_mutex_lock(&_statusLock);
	for (unsigned int i = 0; i < InitialNumberOfThreads; ++i)
	{
		_threads[i] = new ThreadPoolWorker(_taskQueue, _statusLock, _exitSignal);
		_threads[i]->start();
	}
	pthread_mutex_unlock(&_statusLock);
}

ThreadPool::~ThreadPool()
{
	destroy(false);
}

void	ThreadPool::addTask(IThreadTask* newTask)
{
	_taskQueue.addTask(newTask);
}

void	ThreadPool::addTask(IThreadTask& newTask)
{
	_taskQueue.addTask(&newTask);
}

void	ThreadPool::waitForCompletion()
{
	_taskQueue.waitForCompletion();
}

void	ThreadPool::destroy(bool waitForCompletion)
{
	if (!waitForCompletion)
		_taskQueue.clear();
	_taskQueue.waitForCompletion();
	for (unsigned int i = 0; i < _threads.size(); ++i)
		_taskQueue.addTask(NULL);
	for (int i = _threads.size() - 1; i >= 0; --i)
	{
		_threads[i]->join();
		delete (_threads[i]);
		_threads.pop_back();
	}
}

void	ThreadPool::addThread()
{
	ThreadPoolWorker*   newThread;

	newThread = new ThreadPoolWorker(_taskQueue, _statusLock, _exitSignal);
	_threads.push_back(newThread);
	newThread->start();
}

void	ThreadPool::removeThread()
{
	pthread_mutex_lock(&_statusLock);
	_taskQueue.addTask(NULL);
	pthread_cond_wait(&_exitSignal, &_statusLock);

	std::vector<ThreadPoolWorker*>::iterator curThread;	
	for (curThread = _threads.begin(); curThread != _threads.end(); ++curThread)
	{
		if ((*curThread)->exitedQueue())
		{
			(*curThread)->join();
			delete (*curThread);
			_threads.erase(curThread);
			break ;
		}
	}
	pthread_mutex_unlock(&_statusLock);
}

int	ThreadPool::threadCount() const
{
	return (_threads.size());
}