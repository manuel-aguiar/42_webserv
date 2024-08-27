/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _LegacyThreadPool.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:03:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 09:28:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*

#ifndef THREADPOOL_HPP

# define THREADPOOL_HPP

// https://stackoverflow.com/questions/22284557/thread-pool-implementation-using-pthreads

# include <pthread.h>
# include <queue>
# include <list>
# include <vector>
# include <iostream>
# include <cstring>


# include "IThreadTask.hpp"

class ThreadPool
{
    public:
        ThreadPool(const int numberOfThreads);
        ~ThreadPool();

        void    destroy(bool interrupt);
        void    addTask(void *(*task)(void *), void *args);
        void    addTask(IThreadTask* newTask);
        void    waitCompletion();
        void    addThread();
        void    removeThread();

    private:

        //private to the ThreadPool
        
        typedef std::queue<IThreadTask, std::list<IThreadTask *> > tasks;


        tasks                                               _tasks;
        std::vector<pthread_t>                              _threads;
        void*                                               _taskPool(void* pool);

        IThreadTask                                          _getTask();

        pthread_mutex_t                                     _executeTask;
        pthread_cond_t                                      _waitTask;
        pthread_cond_t                                      _endedTask;

        const unsigned int                                  _numberOfThreads;
        unsigned int                                        _busyThreads;

        bool                                                _FlagDestroy;
        bool                                                _FlagInterrupt;

        unsigned int                                        _ErrorLastGoodThread;
                                               
        //boilerplate
        ThreadPool();
        ThreadPool(const ThreadPool& copy);
        ThreadPool& operator=(const ThreadPool& assign);    
};

#endif


*/