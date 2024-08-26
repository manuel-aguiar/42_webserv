/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTask.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 11:34:48 by mmaria-d         ###   ########.fr       */
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
        ThreadTask();
        ~ThreadTask();
        ThreadTask(const ThreadTask& copy);
        ThreadTask& operator=(const ThreadTask& assign);

        void        execute() const;

    private:
        task        _task;
        args        _args;

        

};

#endif