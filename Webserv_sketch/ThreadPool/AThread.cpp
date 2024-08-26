/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AThread.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:26:47 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/08/26 13:28:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AThread.hpp"

AThread::AThread()
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