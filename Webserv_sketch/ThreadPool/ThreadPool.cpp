/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:30:00 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 10:38:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadPool.hpp"

ThreadPool::ThreadPool(unsigned int InitialNumberOfThreads)
{
    ThreadPoolWorker*   newThread;

    for (int i = 0; i < InitialNumberOfThreads; ++i)
    {
        newThread = new ThreadPoolWorker(_taskQueue);
        _threads.push_back(newThread);
        newThread->start();
    }
}

ThreadPool::~ThreadPool()
{
    destroy(false);
}

void    ThreadPool::addTask(IThreadTask* newTask)
{
    _taskQueue.addTask(newTask);
}

void    ThreadPool::addTask(IThreadTask& newTask)
{
    _taskQueue.addTask(&newTask);
}

void    ThreadPool::waitForCompletion()
{
    _taskQueue.waitForCompletion();
}

void    ThreadPool::destroy(bool waitForCompletion)
{
    if (waitForCompletion)
        _taskQueue.waitForCompletion();
    else
        _taskQueue.clear();

    for (int i = 0; i < _threads.size(); ++i)
        _taskQueue.addTask(NULL);
    for (int i = 0; i < _threads.size(); ++i)
    {
        _threads[i]->join();
        delete (_threads[i]);
    }
        
}