/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clock.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 14:46:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clock.hpp"

Clock::Clock() : _time (0)
{
    
}

Clock::~Clock()
{

}





Clock::Clock(const Clock& copy)
{
    _time = copy._time;
}

Clock& Clock::operator=(const Clock& assign)
{
    _time = assign._time;
    return (*this);
}