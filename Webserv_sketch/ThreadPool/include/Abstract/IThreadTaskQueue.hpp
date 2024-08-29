/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IThreadTaskQueue.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:31:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/29 12:17:31 by manuel           ###   ########.fr       */
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
        virtual void                finishTask(IThreadTask* delTask) = 0;
    
    protected:
        virtual IThreadTask*        cloneTask(const IThreadTask* base) = 0;
};



#endif