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

	for (unsigned int i = 0; i < InitialNumberOfThreads; ++i)
	{
		_threads[i] = new ThreadPoolWorker(_taskQueue);
		_threads[i]->start();
	}

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

	newThread = new ThreadPoolWorker(_taskQueue);
	_threads.push_back(newThread);
	newThread->start();
}

int	ThreadPool::threadCount() const
{
	return (_threads.size());
}