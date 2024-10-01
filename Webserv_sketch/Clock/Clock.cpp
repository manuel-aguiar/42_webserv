/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clock.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 18:47:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clock.hpp"
#include "../Globals/Globals.hpp"

Clock::Clock(Globals* globals) : _globals(globals), _elapsed_ms(0)
{
        
}

Clock::~Clock()
{
    
}

void    Clock::setGlobals(Globals& globals)
{
    _globals = &globals;
}

void   Clock::start()
{
    if (!_get_time())
        return ;
}

void    Clock::update()
{
    if (!_get_time())
        return ;
    _elapsed_ms = (_now.tv_sec - _start.tv_sec) * 1000 + (_now.tv_usec - _start.tv_usec) / 1000;
}

inline
int Clock::_get_time()
{
    if (::gettimeofday(&_now, NULL) == -1)
    {
        _globals->_logFile->record("gettimeofday(): " + std::string(std::strerror(errno)));
        return (0);
    }
    return (1);    
}

