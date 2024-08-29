/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedTask.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 08:56:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SharedTask.hpp"

SharedTask::SharedTask(pthread_mutex_t& mutex, int index) : 
    _mutex(mutex),
    _index(index)
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "SharedTask Constructor Called" << std::endl;
    #endif
}

SharedTask::~SharedTask()
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "SharedTask dSTRUCTOR Called" << std::endl;
    #endif    
}
SharedTask::SharedTask(const SharedTask& copy) :
    _mutex(copy._mutex),
    _index(copy._index)
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "SharedTask Constructor Called" << std::endl;
    #endif
}


SharedTask&  SharedTask::operator=(const SharedTask& copy)
{
    if (this == &copy)
        return (*this);
    *this = copy;
    return (*this);
}

void    SharedTask::execute() const
{
    pthread_mutex_lock(&_mutex);
    std::cout << "Printing my number: " << _index << std::endl;
    pthread_mutex_unlock(&_mutex);
}

IThreadTask*    SharedTask::clone() const
{
    return (new SharedTask(*this));
}