/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:30:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 10:35:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOL_HPP

# define THREADPOOL_HPP

# include <vector>

# include "IThreadPool.hpp"
# include "IThreadTask.hpp"

# include "ThreadTaskQueue.hpp"
# include "ThreadPoolWorker.hpp"

// must know about threads, tasks, and queues
// can handle any task that complies with the IThreadTask interface
// must take a predefined queue and threadworker, both complying 
// with IThreadTaskQueue and IThreadPoolWorker interfaces, respectively
// queue and thread are tightely linked

class ThreadPool : public IThreadPool
{
    public:
        ThreadPool(unsigned int InitialNumberOfThreads);
        ~ThreadPool();

        void    addTask(IThreadTask* task);
        void    addTask(IThreadTask& task);
        void    waitForCompletion();
        void    destroy(bool waitForCompletion);
        void    addThread();
        void    removeThread();

    private:
        ThreadTaskQueue                         _taskQueue;
        std::vector<ThreadPoolWorker *>         _threads;

};






#endif