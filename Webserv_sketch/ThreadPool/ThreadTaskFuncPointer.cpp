/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadTaskFuncPointer.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 09:09:48 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 08:26:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ThreadTaskFuncPointer.hpp"

void    ThreadTaskFuncPointer::execute() const
{
    _task(_args);
}

ThreadTaskFuncPointer::ThreadTaskFuncPointer(void* (*task)(void* ), void* args) : 
    _task(task),
    _args(args)
{
    assert(_task != NULL && _args != NULL);
}

ThreadTaskFuncPointer::~ThreadTaskFuncPointer();
ThreadTaskFuncPointer::ThreadTaskFuncPointer(const ThreadTaskFuncPointer& copy);