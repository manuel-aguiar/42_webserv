/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTask.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 08:56:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Concrete/ThreadTask.hpp"

ThreadTask::ThreadTask(std::vector<int>& vec, int index) : 
    _vec(vec),
    _index(index)
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "ThreadTask Constructor Called" << std::endl;
    #endif
}

ThreadTask::~ThreadTask()
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "ThreadTask dSTRUCTOR Called" << std::endl;
    #endif    
}
ThreadTask::ThreadTask(const ThreadTask& copy) :
    _vec(copy._vec),
    _index(copy._index)
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "ThreadTask Constructor Called" << std::endl;
    #endif
}


ThreadTask&  ThreadTask::operator=(const ThreadTask& copy)
{
    if (this == &copy)
        return (*this);
    *this = copy;
    return (*this);
}

void    ThreadTask::execute() const
{
    _vec[_index] = _index;
}

IThreadTask*    ThreadTask::clone() const
{
    return (new ThreadTask(*this));
}