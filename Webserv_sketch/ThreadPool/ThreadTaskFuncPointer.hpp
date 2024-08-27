/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTaskFuncPointer.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 08:18:25 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADTASK_HPP

# define THREADTASK_HPP

//class ThreadPool;

# include <pthread.h>
# include <iostream>
# include <cstring>

# include "ThreadPool.hpp"


class ThreadTaskFuncPointer : public IThreadTask
{
    public:
        typedef void* (*task)(void* args);
        typedef void*   args;

        ThreadTaskFuncPointer(task, args);
        
        ~ThreadTaskFuncPointer();
        ThreadTaskFuncPointer(const ThreadTaskFuncPointer& copy);
        

        void            execute() const;
        IThreadTask*    clone();
        void            run();

    private:
        task        _task;
        args        _args;

        ThreadTaskFuncPointer();
        ThreadTaskFuncPointer& operator=(const ThreadTaskFuncPointer& assign);
        

};

#endif