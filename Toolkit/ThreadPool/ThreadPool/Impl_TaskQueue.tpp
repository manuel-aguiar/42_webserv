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

#ifndef IMPL_TASKQUEUE_TPP

# define IMPL_TASKQUEUE_TPP

#include "../ThreadPool/ThreadPool.hpp"

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::TaskQueue()
    : m_tasksExecuting(0)
{
    pthread_mutex_init(&m_taskAccess, NULL);
    pthread_cond_init(&m_newTaskSignal, NULL);
    pthread_cond_init(&m_allTasksDone, NULL);
	pthread_cond_init(&m_fullQueue, NULL);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::~TaskQueue()
{
	pthread_mutex_destroy(&m_taskAccess);
	pthread_cond_destroy(&m_newTaskSignal);
	pthread_cond_destroy(&m_allTasksDone);
	pthread_cond_destroy(&m_fullQueue);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::TaskQueue(const TaskQueue& copy)  {(void)copy;}

template <size_t ThreadBacklog, size_t TaskBacklog>
typename ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue& 
ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::operator=(const TaskQueue& assign)  {(void)assign; return (*this);}


template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::finishTask(IThreadTask* delTask)
{
	(void)delTask;
	pthread_mutex_lock(&m_taskAccess);
	m_tasksExecuting--;
	if (m_tasks.size() == 0 && m_tasksExecuting == 0)
		pthread_cond_signal(&m_allTasksDone);
	pthread_mutex_unlock(&m_taskAccess);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
bool	ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::addTask(IThreadTask* newTask, bool waitForSlot)
{
	pthread_mutex_lock(&m_taskAccess);

	if (m_tasks.size() == TaskBacklog)
	{
		if (!waitForSlot)
		{
			pthread_mutex_unlock(&m_taskAccess);
			return (false);
		}
		else
			pthread_cond_wait(&m_fullQueue, &m_taskAccess);
	}

	m_tasks.push_back(newTask);
	pthread_cond_signal(&m_newTaskSignal);
	pthread_mutex_unlock(&m_taskAccess);

	return (true);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
IThreadTask*	 ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::acquireTask()
{
	IThreadTask *toExecute;

	pthread_mutex_lock(&m_taskAccess);
	while (m_tasks.size() == 0)
		pthread_cond_wait(&m_newTaskSignal, &m_taskAccess);
	toExecute = m_tasks.front();
	m_tasks.pop_front();
	m_tasksExecuting += (toExecute != NULL);
	if (m_tasks.size() == TaskBacklog - 1)
		pthread_cond_signal(&m_fullQueue);
	pthread_mutex_unlock(&m_taskAccess);
	return (toExecute);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::clear()
{
	pthread_mutex_lock(&m_taskAccess);
	m_tasks.clear();
	pthread_mutex_unlock(&m_taskAccess);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
void	ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::waitForCompletion()
{   
	pthread_mutex_lock(&m_taskAccess);
	while (m_tasks.size() != 0 || m_tasksExecuting)
		pthread_cond_wait(&m_allTasksDone, &m_taskAccess);
	pthread_mutex_unlock(&m_taskAccess);
}

template <size_t ThreadBacklog, size_t TaskBacklog>
size_t	ThreadPool<ThreadBacklog, TaskBacklog>::TaskQueue::getTaskCount()
{
	size_t result;
	
	pthread_mutex_lock(&m_taskAccess);
	result = m_tasks.size() + m_tasksExecuting;
	pthread_mutex_unlock(&m_taskAccess);
	return (result);
}

#endif