/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:03:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 09:14:45 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOL_HPP

# define THREADPOOL_HPP

# include <pthread.h>
# include <list>
# include <vector>

# include "ThreadTask.hpp"

class ThreadPool
{
    public:
        ThreadPool(const int numberOfThreads);
        ~ThreadPool();

    private:


        std::list<ThreadTask>    tasks;
        std::vector<pthread_t>   threads;


        //boilerplate
        ThreadPool();
        ThreadPool(const ThreadPool& copy);
        ThreadPool& operator=(const ThreadPool& assign);    
};

#endif