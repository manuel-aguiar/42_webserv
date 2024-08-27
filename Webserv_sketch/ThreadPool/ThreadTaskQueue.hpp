/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTaskQueue.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:33:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 09:37:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADTASKQUEUE_HPP

# define THREADTASKQUEUE_HPP

# include <pthread.h>
# include <queue>
# include <list>

# include "IThreadTaskQueue.hpp"



class ThreadTaskQueue : public IThreadTaskQueue
{
    public:
        ThreadTaskQueue();
        ~ThreadTaskQueue();

        void                addTask(IThreadTask* newTask);
        IThreadTask*        getTask();
        void                clear();
        void                waitForCompletion();        

    private:
        std::list<IThreadTask*>                             _queue;
        pthread_mutex_t                                     _taskAccess;
        pthread_cond_t                                      _newTaskSignal;                                   
        pthread_cond_t                                      _allTasksDone;


        ThreadTaskQueue(const ThreadTaskQueue& copy);
        ThreadTaskQueue& operator=(const ThreadTaskQueue& assign);
};



#endif