/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:30:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/29 10:01:17 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOL_HPP

# define THREADPOOL_HPP

# include <vector>

# include "../Abstract/IThreadPool.hpp"
# include "../Abstract/IThreadTask.hpp"

# include "../Concrete/ThreadTask.tpp"
# include "../Concrete/ThreadTaskQueue.hpp"
# include "../Concrete/ThreadPoolWorker.hpp"

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

        //not implemented
        //void    removeThread();
        
        int     threadCount() const;

    private:
        ThreadTaskQueue                         _taskQueue;
        std::vector<ThreadPoolWorker *>         _threads;

    


    //public template specializations for ThreadTask
    public:
        template<typename Return, typename Args>
        void addTask(Return (*function)(Args), Args args, Return* placeReturn = NULL)
        {
            IThreadTask* task = new ThreadTask<Return (*)(Args)>(function, args, placeReturn);
            addTask(task);
        }

        template<typename Return>
        void addTask(Return (*function)(void), Return* placeReturn = NULL)
        {
            IThreadTask* task = new ThreadTask<Return (*)(void)>(function, placeReturn);
            addTask(task);
        }

        template<typename Args>
        void addTask(void (*function)(Args), Args args)
        {
            IThreadTask* task = new ThreadTask<void (*)(Args)>(function, args);
            addTask(task);
        }

        void addTask(void (*function)(void))
        {
            IThreadTask* task = new ThreadTask<void (*)(void)>(function);
            addTask(task);
        }

        template<typename Class, typename Args, typename Return>
        void addTask(Class& instance, Return (Class::*function)(Args), Args args, Return* placeReturn = NULL)
        {
            IThreadTask* task = new ThreadTask<Return (Class::*)(Args)>(instance, function, args, placeReturn);
            addTask(task);
        }

        template<typename Class, typename Args>
        void addTask(Class& instance, void (Class::*function)(Args), Args args)
        {
            IThreadTask* task = new ThreadTask<void (Class::*)(Args)>(instance, function, args);
            addTask(task);
        }

        template <typename Class, typename Return>
        void addTask(Class& instance, Return (Class::*function)(void), Return* placeReturn = NULL)
        {
            IThreadTask* task = new ThreadTask<Return (Class::*)(void)>(instance, function, placeReturn);
            addTask(task);
        }

        template<typename Class>
        void addTask(Class& instance, void (Class::*function)(void))
        {
            IThreadTask* task = new ThreadTask<void (Class::*)(void)>(instance, function);
            addTask(task);
        }       

};




#endif