/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AThread.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:26:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 13:43:29 by mmaria-d         ###   ########.fr       */
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
