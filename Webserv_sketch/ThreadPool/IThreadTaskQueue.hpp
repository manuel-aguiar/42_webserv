/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IThreadTaskQueue.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:31:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 14:13:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITHREADTASKQUEUE_HPP

# define THREADTASKQUEUE_HPP

# include "IThreadTask.hpp"

class IThreadTaskQueue
{
    virtual ~IThreadTaskQueue();
    virtual void                addTask(const IThreadTask* newTask) = 0;
    virtual IThreadTask*        getTask() = 0;
    virtual void                clear() = 0;
    virtual void                waitForCompletion() = 0;
};



#endif