/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:30:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/29 16:47:24 by mmaria-d         ###   ########.fr       */
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
            ThreadTask<Return (*)(Args)> task(function, args, placeReturn);
            _taskQueue.addTask(&task);
        }

        template<typename Return>
        void addTask(Return (*function)(void), Return* placeReturn = NULL)
        {
            ThreadTask<Return (*)(void)> task(function, placeReturn);
            _taskQueue.addTask(&task);
        }

        template<typename Args>
        void addTask(void (*function)(Args), Args args)
        {
            ThreadTask<void (*)(Args)> task (function, args);
            _taskQueue.addTask(&task);
        }

        void addTask(void (*function)(void))
        {
            ThreadTask<void (*)(void)> task(function);
            _taskQueue.addTask(&task);
        }

        template<typename Class, typename Args, typename Return>
        void addTask(Class& instance, Return (Class::*function)(Args), Args args, Return* placeReturn = NULL)
        {
            ThreadTask<Return (Class::*)(Args)> task(instance, function, args, placeReturn);
            _taskQueue.addTask(&task);
        }

        template<typename Class, typename Args>
        void addTask(Class& instance, void (Class::*function)(Args), Args args)
        {
            ThreadTask<void (Class::*)(Args)> task(instance, function, args);
            _taskQueue.addTask(&task);
        }

        template <typename Class, typename Return>
        void addTask(Class& instance, Return (Class::*function)(void), Return* placeReturn = NULL)
        {
            ThreadTask<Return (Class::*)(void)> task(instance, function, placeReturn);
            _taskQueue.addTask(&task);
        }

        template<typename Class>
        void addTask(Class& instance, void (Class::*function)(void))
        {
            ThreadTask<void (Class::*)(void)> task(instance, function);
            _taskQueue.addTask(&task);
        }

        template<typename Class, typename Args, typename Return>
        void addTask(const Class& instance, Return (Class::*function)(Args) const, Args args, Return* placeReturn = NULL)
        {
            ThreadTask<Return (Class::*)(Args) const> task(instance, function, args, placeReturn);
            _taskQueue.addTask(&task);
        }

        template<typename Class, typename Args>
        void addTask(const Class& instance, void (Class::*function)(Args) const, Args args)
        {
            ThreadTask<void (Class::*)(Args) const> task(instance, function, args);
            _taskQueue.addTask(&task);
        }

        template <typename Class, typename Return>
        void addTask(const Class& instance, Return (Class::*function)(void) const, Return* placeReturn = NULL)
        {
            ThreadTask<Return (Class::*)(void) const> task(instance, function, placeReturn);
            _taskQueue.addTask(&task);
        }

        template<typename Class>
        void addTask(const Class& instance, void (Class::*function)(void) const)
        {
            ThreadTask<void (Class::*)(void) const> task(instance, function);
            _taskQueue.addTask(&task);
        }         

};




#endif