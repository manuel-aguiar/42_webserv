/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ThreadTaskQueue.cpp								:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: manuel <manuel@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 14:06:33 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/29 12:10:43 by manuel		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../include/Concrete/ThreadTaskQueue.hpp"

ThreadTaskQueue::ThreadTaskQueue() :
	_tasksExecuting(0)
{
	pthread_mutex_init(&_taskAccess, NULL);
	pthread_cond_init(&_newTaskSignal, NULL);
	pthread_cond_init(&_allTasksDone, NULL);
}

ThreadTaskQueue::~ThreadTaskQueue()
{
	pthread_mutex_destroy(&_taskAccess);
	pthread_cond_destroy(&_newTaskSignal);
	pthread_cond_destroy(&_allTasksDone);
}

ThreadTaskQueue::ThreadTaskQueue(const ThreadTaskQueue& copy)  {(void)copy;}
ThreadTaskQueue& ThreadTaskQueue::operator=(const ThreadTaskQueue& assign)  {(void)assign; return (*this);}

IThreadTask*	ThreadTaskQueue::cloneTask(const IThreadTask* newTask)
{
	if (!newTask)
		return (NULL);
	return (newTask->clone());
}

void			ThreadTaskQueue::finishTask(IThreadTask* delTask)
{
	if (delTask)
		delete (delTask);
	pthread_mutex_lock(&_taskAccess);
	_tasksExecuting--;
	if (_tasks.size() == 0 && _tasksExecuting == 0)
		pthread_cond_signal(&_allTasksDone);
	pthread_mutex_unlock(&_taskAccess);
}

void	ThreadTaskQueue::addTask(const IThreadTask* newTask)
{
	pthread_mutex_lock(&_taskAccess);
	_tasks.push_back(cloneTask(newTask));
	pthread_cond_signal(&_newTaskSignal);
	pthread_mutex_unlock(&_taskAccess);
}

IThreadTask*	 ThreadTaskQueue::getTask()
{
	IThreadTask *toExecute;

	pthread_mutex_lock(&_taskAccess);
	while (_tasks.empty())
		pthread_cond_wait(&_newTaskSignal, &_taskAccess);
	toExecute = _tasks.front();
	_tasks.pop_front();
	_tasksExecuting += (toExecute != NULL);
	pthread_mutex_unlock(&_taskAccess);
	return (toExecute);
}

void	ThreadTaskQueue::clear()
{
	pthread_mutex_lock(&_taskAccess);
	_tasks.clear();
	pthread_mutex_unlock(&_taskAccess);
}

void	ThreadTaskQueue::waitForCompletion()
{   
	pthread_mutex_lock(&_taskAccess);
	while (!_tasks.empty() || _tasksExecuting)
		pthread_cond_wait(&_allTasksDone, &_taskAccess);
	pthread_mutex_unlock(&_taskAccess);
}

int	 ThreadTaskQueue::taskCount()
{
	int result;
	
	pthread_mutex_lock(&_taskAccess);
	result = _tasks.size() + _tasksExecuting;
	pthread_mutex_unlock(&_taskAccess);
	return (result);
}