/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:03:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 10:24:29 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOL_HPP

# define THREADPOOL_HPP

# include <pthread.h>
# include <queue>
# include <list>
# include <vector>
# include <iostream>
# include <cstring>

class ThreadPool
{
    public:
        ThreadPool(const int numberOfThreads);
        ~ThreadPool();

        void    destroy(bool interrupt);

    private:

        //private to the ThreadPool
        class ThreadTask;

        std::queue<ThreadTask, std::list<ThreadTask> >      _tasks;
        std::vector<pthread_t>                              _threads;

        pthread_mutex_t                                     _executeTask;
        pthread_cond_t                                      _waitTask;
        pthread_cond_t                                      _endedTask;

        const unsigned int                                  _numberOfThreads;
        unsigned int                                        _busyThreads;
        unsigned int                                        _numberOfTasks;

        bool                                                _FlagDestroy;
        bool                                                _FlagInterrupt;

        unsigned int                                        _ErrorLastGoodThread;
                                               
        //boilerplate
        ThreadPool();
        ThreadPool(const ThreadPool& copy);
        ThreadPool& operator=(const ThreadPool& assign);    
};

#endif