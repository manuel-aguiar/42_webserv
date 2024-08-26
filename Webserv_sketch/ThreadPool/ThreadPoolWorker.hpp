/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPoolWorker.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:29:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 13:59:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOLWORKER_HPP

# define THREADPOOLWORKER_HPP

# include "AThread.hpp"
# include "ThreadTaskQueue.hpp"

class ThreadPoolWorker : public AThread
{
    public:
        ThreadPoolWorker(const ThreadTaskQueue& queue);
        ~ThreadPoolWorker();
        

    private:

        ThreadTaskQueue&    queue;

        ThreadPoolWorker();
        ThreadPoolWorker(const ThreadPoolWorker& copy);
        ThreadPoolWorker& operator=(const ThreadPoolWorker& assign);
};


#endif