/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPoolWorker.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:29:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 08:10:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOLWORKER_HPP

# define THREADPOOLWORKER_HPP

# include "AThread.hpp"
# include "IThreadTask.hpp"
# include "IThreadTaskQueue.hpp"

class ThreadPoolWorker : public AThread
{
    public:
        ThreadPoolWorker(IThreadTaskQueue& queue);
        ~ThreadPoolWorker();

    protected:
        void    run();

    private:
        IThreadTaskQueue&    _queue;
        IThreadTask*         _curTask;
        ThreadPoolWorker(const ThreadPoolWorker& copy);
        ThreadPoolWorker& operator=(const ThreadPoolWorker& assign);
};


#endif