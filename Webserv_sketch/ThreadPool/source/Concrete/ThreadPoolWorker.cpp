/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ThreadPoolWorker.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 07:56:13 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/29 08:31:38 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../include/Concrete/ThreadPoolWorker.hpp"

ThreadPoolWorker::ThreadPoolWorker(IThreadTaskQueue& queue) :
    AThread(),
    _queue(queue),
    _curTask(NULL)
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "ThreadPoolWorker Constructor Called" << std::endl;
    #endif
}

ThreadPoolWorker::~ThreadPoolWorker()
{
    #ifdef DEBUG_CONSTRUCTOR
        std::cout << "ThreadPoolWorker Destructor Called" << std::endl;
    #endif    
}

void    ThreadPoolWorker::run()
{   
    while ((_curTask = _queue.getTask()))
    {
        _curTask->execute();
        _queue.finishTask(_curTask);
    }
}

ThreadPoolWorker::ThreadPoolWorker(const ThreadPoolWorker& copy) : 
    AThread(copy),
    _queue(copy._queue),
    _curTask(NULL) {}
    
ThreadPoolWorker& ThreadPoolWorker::operator=(const ThreadPoolWorker& assign)  {(void)assign; return (*this);}


