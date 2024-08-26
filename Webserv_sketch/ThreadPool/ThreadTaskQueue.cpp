/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTaskQueue.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:06:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 14:28:58 by mmaria-d         ###   ########.fr       */
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

ThreadTaskQueue::ThreadTaskQueue(const ThreadTaskQueue& copy)
{

}

ThreadTaskQueue& ThreadTaskQueue::operator=(const ThreadTaskQueue& copy)
{
    
}

void    ThreadTaskQueue::addTask(const IThreadTask* newTask)
{
    
}



void    ThreadTaskQueue::clear()
{
    _queue.clear();
}

void    ThreadTaskQueue::waitForCompletion()
{
    pthread_mutex_lock(&_taskAccess);
    while (!_queue.empty())
        pthread_cond_wait(&_allTasksDone, &_taskAccess);
    pthread_mutex_unlock(&_taskAccess);
}