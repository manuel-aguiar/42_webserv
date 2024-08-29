/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IThreadPool.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 08:52:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 10:42:45 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITHREADPOOL_HPP

# define ITHREADPOOL_HPP

#include "IThreadTask.hpp"

class IThreadPool
{
    public:
        virtual ~IThreadPool() {};
        virtual void    addTask(IThreadTask* newTask)       = 0;
        virtual void    destroy(bool waitForCompletion)     = 0;

};



#endif