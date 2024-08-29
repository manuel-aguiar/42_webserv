/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedTask.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:46 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 09:21:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAREDTASK_HPP

# define SHAREDTASK_HPP

//class ThreadPool;

# include <pthread.h>
# include <iostream>
# include <cstring>
# include <cassert>

# include "../Abstract/IThreadTask.hpp"
# include <vector>
# include <pthread.h>

class SharedTask : public IThreadTask
{
    public:
        SharedTask(pthread_mutex_t& mutex, int index);
        ~SharedTask();
        SharedTask(const SharedTask& copy);
        SharedTask& operator=(const SharedTask& assign);

        void            execute() const;
        IThreadTask*    clone() const;

    private:
        pthread_mutex_t&    _mutex;
        int                 _index;

        
        

};

#endif