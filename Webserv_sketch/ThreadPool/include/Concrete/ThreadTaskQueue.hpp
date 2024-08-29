/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTaskQueue.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:33:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/29 12:12:12 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADTASKQUEUE_HPP

# define THREADTASKQUEUE_HPP

# include <pthread.h>
# include <semaphore.h>
# include <queue>
# include <list>

# include "../Abstract/IThreadTaskQueue.hpp"



class ThreadTaskQueue : public IThreadTaskQueue
{
    public:
        ThreadTaskQueue();
        ~ThreadTaskQueue();

        void                addTask(const IThreadTask* newTask);
        IThreadTask*        getTask();
        void                clear();
        void                waitForCompletion();        
        void                finishTask(IThreadTask* delTask);

        int                 taskCount();

    private:
        IThreadTask*        cloneTask(const IThreadTask* base);
        
        std::list<IThreadTask*>                             _tasks;
        unsigned int                                        _tasksExecuting;
        pthread_mutex_t                                     _taskAccess;
        pthread_cond_t                                      _newTaskSignal;                                   
        pthread_cond_t                                      _allTasksDone;                                         

        ThreadTaskQueue(const ThreadTaskQueue& copy);
        ThreadTaskQueue& operator=(const ThreadTaskQueue& assign);
};



#endif