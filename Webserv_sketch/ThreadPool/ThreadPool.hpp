/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPool.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 09:30:02 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 09:37:29 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOL_HPP

# define THREADPOOL_HPP

# include <vector>

# include "IThreadPool.hpp"
# include "ThreadTaskQueue.hpp"


class ThreadPool : public IThreadPool
{
    public:
        ThreadPool(/* args */);
        ~ThreadPool();
    private:
        ThreadTaskQueue     queue;
        std::vector

};






#endif