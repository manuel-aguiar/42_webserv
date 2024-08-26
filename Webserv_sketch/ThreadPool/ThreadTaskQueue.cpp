/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTaskQueue.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:06:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 14:18:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadTaskQueue.hpp"









void    ThreadTaskQueue::clear()
{
    _queue.clear();
}

void    ThreadTaskQueue::waitForCompletion()
{
    pthread_mutex_lock(&_taskAccess);
    while (!_queue.empty())
        pthread_cond_wait(&_allTasksDone, &_taskAccess);
    pthread_mutex_unlock(&_taskAccess);
}