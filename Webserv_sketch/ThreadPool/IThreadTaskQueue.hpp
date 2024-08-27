/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IThreadTaskQueue.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:31:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 10:49:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITHREADTASKQUEUE_HPP

# define ITHREADTASKQUEUE_HPP

# include "IThreadTask.hpp"

//knows only about tasks

class IThreadTaskQueue
{
    public:
        virtual ~IThreadTaskQueue() {};
        virtual void                addTask(const IThreadTask* newTask) = 0;
        virtual IThreadTask*        getTask() = 0;
        virtual void                clear() = 0;
        virtual void                waitForCompletion() = 0;
        virtual void                deleteTask(IThreadTask* delTask) = 0;
    
    protected:
        virtual IThreadTask*        cloneTask(const IThreadTask* base) = 0;
};



#endif