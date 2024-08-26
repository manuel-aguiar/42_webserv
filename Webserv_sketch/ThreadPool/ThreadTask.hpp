/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTask.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 10:04:54 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADTASK_HPP

# define THREADTASK_HPP

//class ThreadPool;

# include <pthread.h>
# include <iostream>
# include <cstring>

# include "ThreadPool.hpp"


class ThreadPool::ThreadTask
{
    public:
        typedef void* (*task)(void* args);
        typedef void*   args;

        ThreadTask(task, args);
        ~ThreadTask();

        const task& getTask() const;
        const args& getArgs() const;
        void setTask(const task& task);
        void setArgs(const args& args);

    private:
        task        task;
        args        args;

        //boilerplate
        ThreadTask();
        ThreadTask(const ThreadTask& copy);
        ThreadTask& operator=(const ThreadTask& assign);
};

#endif