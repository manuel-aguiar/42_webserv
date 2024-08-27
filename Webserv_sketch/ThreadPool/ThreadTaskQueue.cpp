/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTaskQueue.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:06:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 10:25:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadTaskQueue.hpp"

ThreadTaskQueue::ThreadTaskQueue()
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

ThreadTaskQueue::ThreadTaskQueue(const ThreadTaskQueue& copy)  {}
ThreadTaskQueue& ThreadTaskQueue::operator=(const ThreadTaskQueue& copy)  {}

IThreadTask*    ThreadTaskQueue::cloneTask(const IThreadTask* newTask)
{
    if (!newTask)
        return (NULL);
    return (newTask->clone());
}

void            ThreadTaskQueue::deleteTask(IThreadTask* delTask)
{
    if (delTask)
        delete (delTask);
}

void    ThreadTaskQueue::addTask(const IThreadTask* newTask)
{
    pthread_mutex_lock(&_taskAccess);
    _tasks.push_back(cloneTask(newTask));
    pthread_cond_signal(&_newTaskSignal);
    pthread_mutex_unlock(&_taskAccess);
}

IThreadTask*     ThreadTaskQueue::getTask()
{
    IThreadTask *toExecute;

    pthread_mutex_lock(&_taskAccess);
    while (_tasks.empty())
        pthread_cond_wait(&_newTaskSignal, &_taskAccess);
    toExecute = _tasks.front();
    _tasks.pop_front();
    pthread_mutex_unlock(&_taskAccess);
    return (toExecute);
}

void    ThreadTaskQueue::clear()
{
    pthread_mutex_lock(&_taskAccess);
    _tasks.clear();
    pthread_mutex_unlock(&_taskAccess);
}

void    ThreadTaskQueue::waitForCompletion()
{
    pthread_mutex_lock(&_taskAccess);
    while (!_tasks.empty())
        pthread_cond_wait(&_allTasksDone, &_taskAccess);
    pthread_mutex_unlock(&_taskAccess);
}