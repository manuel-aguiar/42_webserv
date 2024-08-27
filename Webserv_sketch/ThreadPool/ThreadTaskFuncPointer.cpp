/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTaskFuncPointer.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 08:56:38 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadTaskFuncPointer.hpp"

ThreadTaskFuncPointer::ThreadTaskFuncPointer(void* (*task)(void* ), void* args) : 
    _task(task),
    _args(args)
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "ThreadTaskFuncPointer Constructor Called" << std::endl;
    #endif
    assert(_task != NULL && _args != NULL);
}

ThreadTaskFuncPointer::~ThreadTaskFuncPointer()
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "ThreadTaskFuncPointer dSTRUCTOR Called" << std::endl;
    #endif    
}
ThreadTaskFuncPointer::ThreadTaskFuncPointer(const ThreadTaskFuncPointer& copy) :
    _task(copy._task),
    _args(copy._args)
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "ThreadTaskFuncPointer Constructor Called" << std::endl;
    #endif
    assert(_task != NULL && _args != NULL);
}


ThreadTaskFuncPointer&  ThreadTaskFuncPointer::operator=(const ThreadTaskFuncPointer& copy)
{
    if (this == &copy)
        return (*this);
    *this = copy;
    return (*this);
}

void    ThreadTaskFuncPointer::execute() const
{
    _task(_args);
}

IThreadTask*    ThreadTaskFuncPointer::clone() const
{
    return (new ThreadTaskFuncPointer(*this));
}