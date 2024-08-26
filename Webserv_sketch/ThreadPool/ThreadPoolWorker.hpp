/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPoolWorker.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:29:44 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 15:47:08 by mmaria-d         ###   ########.fr       */
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
        ThreadPoolWorker(const IThreadTaskQueue& queue);
        ~ThreadPoolWorker();

    protected:
        void    run();

    private:

        IThreadTaskQueue&    queue;

        ThreadPoolWorker();
        ThreadPoolWorker(const ThreadPoolWorker& copy);
        ThreadPoolWorker& operator=(const ThreadPoolWorker& assign);
};


#endif