/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   TaskQueue.cpp								:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: manuel <manuel@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/26 14:06:33 by mmaria-d		  #+#	#+#			 */
/*   Updated: 2024/08/29 12:10:43 by manuel		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "TaskQueue.hpp"

TaskQueue::TaskQueue() :
	m_tasksExecuting(0)
{
	pthread_mutex_init(&m_taskAccess, NULL);
	pthread_cond_init(&m_newTaskSignal, NULL);
	pthread_cond_init(&m_allTasksDone, NULL);
}

TaskQueue::~TaskQueue()
{
	pthread_mutex_destroy(&m_taskAccess);
	pthread_cond_destroy(&m_newTaskSignal);
	pthread_cond_destroy(&m_allTasksDone);
}

TaskQueue::TaskQueue(const TaskQueue& copy)  {(void)copy;}
TaskQueue& TaskQueue::operator=(const TaskQueue& assign)  {(void)assign; return (*this);}

void			TaskQueue::finishTask(IThreadTask* delTask)
{
	if (delTask)
		delete (delTask);
	pthread_mutex_lock(&m_taskAccess);
	m_tasksExecuting--;
	if (m_tasks.size() == 0 && m_tasksExecuting == 0)
		pthread_cond_signal(&m_allTasksDone);
	pthread_mutex_unlock(&m_taskAccess);
}

void	TaskQueue::addTask(IThreadTask* newTask)
{
	pthread_mutex_lock(&m_taskAccess);
	m_tasks.push_back(newTask);
	pthread_cond_signal(&m_newTaskSignal);
	pthread_mutex_unlock(&m_taskAccess);
}

IThreadTask*	 TaskQueue::getTask()
{
	IThreadTask *toExecute;

	pthread_mutex_lock(&m_taskAccess);
	while (m_tasks.size() == 0)
		pthread_cond_wait(&m_newTaskSignal, &m_taskAccess);
	toExecute = m_tasks.front();
	m_tasks.pop_front();
	m_tasksExecuting += (toExecute != NULL);
	pthread_mutex_unlock(&m_taskAccess);
	return (toExecute);
}

void	TaskQueue::clear()
{
	pthread_mutex_lock(&m_taskAccess);
	m_tasks.clear();
	pthread_mutex_unlock(&m_taskAccess);
}

void	TaskQueue::waitForCompletion()
{   
	pthread_mutex_lock(&m_taskAccess);
	while (m_tasks.size() != 0 || m_tasksExecuting)
		pthread_cond_wait(&m_allTasksDone, &m_taskAccess);
	pthread_mutex_unlock(&m_taskAccess);
}

int	 TaskQueue::taskCount()
{
	int result;
	
	pthread_mutex_lock(&m_taskAccess);
	result = m_tasks.size() + m_tasksExecuting;
	pthread_mutex_unlock(&m_taskAccess);
	return (result);
}