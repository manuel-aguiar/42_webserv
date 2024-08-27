/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AThread.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:26:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/27 08:12:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AThread.hpp"

AThread::AThread() : 
    _state(EThread_Unitialized),
    _thread(0)
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "AThread Constructor Called" << std::endl;
    #endif
}

AThread::~AThread()
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "AThread Destructor Called" << std::endl;
    #endif   
}

AThread::AThread(const AThread& copy) : 
    _state(copy._state),
    _thread(copy._thread)
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "AThread Copy Constructor Called" << std::endl;
    #endif       
}

AThread& AThread::operator=(const AThread& assign)
{
    if (&assign == this)
        return (*this);
    *this = assign;
    return (*this);
}

void    AThread::start()
{
    if (_state == EThread_Initialized)
    {
        std::cerr << "Thread already initialized" << std::endl;
        return ;
    }
    if (pthread_create(&_thread, NULL, ThreadFunction, this))
    {
        std::cerr << "Thread failed to create" << std::endl;
        return ;
    }
    _state = EThread_Initialized;
}

void    AThread::join()
{
    pthread_join(_thread, NULL);
    _state = EThread_Joined;
}

void*   AThread::ThreadFunction(void* args)
{
    AThread* thread = (AThread *)args;
    thread->run();
    return (NULL);
}