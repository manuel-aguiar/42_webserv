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
	m_tasksExecuting(0)
{
	pthread_mutex_init(&m_taskAccess, NULL);
	pthread_cond_init(&m_newTaskSignal, NULL);
	pthread_cond_init(&m_allTasksDone, NULL);
}

ThreadTaskQueue::~ThreadTaskQueue()
{
	pthread_mutex_destroy(&m_taskAccess);
	pthread_cond_destroy(&m_newTaskSignal);
	pthread_cond_destroy(&m_allTasksDone);
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
	pthread_mutex_lock(&m_taskAccess);
	m_tasksExecuting--;
	if (m_tasks.size() == 0 && m_tasksExecuting == 0)
		pthread_cond_signal(&m_allTasksDone);
	pthread_mutex_unlock(&m_taskAccess);
}

void	ThreadTaskQueue::addTask(const IThreadTask* newTask)
{
	pthread_mutex_lock(&m_taskAccess);
	m_tasks.push_back(cloneTask(newTask));
	pthread_cond_signal(&m_newTaskSignal);
	pthread_mutex_unlock(&m_taskAccess);
}

IThreadTask*	 ThreadTaskQueue::getTask()
{
	IThreadTask *toExecute;

	pthread_mutex_lock(&m_taskAccess);
	while (m_tasks.empty())
		pthread_cond_wait(&m_newTaskSignal, &m_taskAccess);
	toExecute = m_tasks.front();
	m_tasks.pop_front();
	m_tasksExecuting += (toExecute != NULL);
	pthread_mutex_unlock(&m_taskAccess);
	return (toExecute);
}

void	ThreadTaskQueue::clear()
{
	pthread_mutex_lock(&m_taskAccess);
	m_tasks.clear();
	pthread_mutex_unlock(&m_taskAccess);
}

void	ThreadTaskQueue::waitForCompletion()
{   
	pthread_mutex_lock(&m_taskAccess);
	while (!m_tasks.empty() || m_tasksExecuting)
		pthread_cond_wait(&m_allTasksDone, &m_taskAccess);
	pthread_mutex_unlock(&m_taskAccess);
}

int	 ThreadTaskQueue::taskCount()
{
	int result;
	
	pthread_mutex_lock(&m_taskAccess);
	result = m_tasks.size() + m_tasksExecuting;
	pthread_mutex_unlock(&m_taskAccess);
	return (result);
}