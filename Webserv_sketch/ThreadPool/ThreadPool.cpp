/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:39:42 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 12:08:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ThreadPool.hpp"
# include "ThreadTask.hpp"

ThreadPool::ThreadPool(const int numberOfThreads) :
    _numberOfThreads(numberOfThreads),
    _FlagDestroy(false),
    _FlagInterrupt(false),
    _ErrorLastGoodThread(numberOfThreads)             
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "ThreadPool Parameter Constructor Called" << std::endl;
    #endif

    _threads.reserve(numberOfThreads);
    std::memset(&_executeTask, 0, sizeof(_executeTask));
    std::memset(&_waitTask, 0, sizeof(_waitTask));
    std::memset(&_endedTask, 0, sizeof(_endedTask));

    if (pthread_mutex_init(&_executeTask, NULL))
        std::cerr << "pthread_mutex_init():" << std::string (std::strerror(errno)) << std::endl;
    if (pthread_cond_init(&_waitTask, NULL))
        std::cerr << "pthread_cond_init():" << std::string (std::strerror(errno)) << std::endl;
    if (pthread_cond_init(&_endedTask, NULL))
        std::cerr << "pthread_cond_init():" << std::string (std::strerror(errno)) << std::endl;
    
    pthread_mutex_lock(&_executeTask);

    for (int i = 0; i < _numberOfThreads; ++i)
    {

    }
}

void    ThreadPool::addTask(void *(*task)(void *), void *args)
{
    ThreadTask newTask(task, args);

    pthread_mutex_lock(&_executeTask);
    _tasks.push(newTask);
    if (_busyThreads < _threads.size())
        pthread_cond_signal(&_waitTask);
    pthread_mutex_unlock(&_executeTask);
}

ThreadPool::ThreadTask     ThreadPool::_getTask()
{
    ThreadTask  task(_tasks.front());

    _tasks.pop();
    return (task);
}


void*   ThreadPool::_taskPool(void* pool)
{
    ThreadPool* tp = (ThreadPool *)pool;
    ThreadTask task;

    while (true)
    {
        pthread_mutex_lock(&tp->_executeTask);
        if (tp->_FlagDestroy && (!tp->_tasks.size() || (tp->_tasks.size() && tp->_FlagInterrupt)))
        {
            pthread_mutex_unlock(&tp->_executeTask);
            break ;
        }
        else if (tp->_tasks.size())
        {
            task = _getTask();
            tp->_busyThreads++;
            pthread_mutex_unlock(&tp->_executeTask);
            task.execute();
            pthread_mutex_lock(&tp->_executeTask);
            tp->_busyThreads--;
            pthread_mutex_unlock(&tp->_executeTask);
            pthread_cond_signal(&tp->_endedTask);
        }
        else
        {
            pthread_cond_wait(&tp->_waitTask, &tp->_executeTask);
            pthread_mutex_unlock(&tp->_executeTask);
        }
    }




    return (NULL);
}

void ThreadPool::destroy(bool interrupt)
{
    //if (!std::memcmp(&_executeTask, &((pthread_mutex_t){}), sizeof(_executeTask)))
    //    return ;
    pthread_mutex_lock(&_executeTask);
    _FlagInterrupt = interrupt;
    _FlagDestroy = true;

    while (_busyThreads > 0)
        pthread_cond_wait(&_endedTask, &_executeTask);
    
    //wake up everyone after setting the destruction flag
    pthread_cond_broadcast(&_waitTask);

    //allow threads to reach the decision section, check flags, and exit their thread function
    pthread_mutex_unlock(&_executeTask);

    //join everyone
    for (int i = 0; i < _ErrorLastGoodThread; ++i)
        pthread_join(_threads[i], NULL);
    
    //clean up mutexes and conditionals
    pthread_mutex_destroy(&_executeTask);
    pthread_cond_destroy(&_waitTask);
    pthread_cond_destroy(&_endedTask);
}