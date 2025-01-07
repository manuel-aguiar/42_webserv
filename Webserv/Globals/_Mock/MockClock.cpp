/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockClock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 09:08:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Clock/Clock.hpp"
#include "../Globals.hpp"


Clock::Clock(Globals* globals) : m_globals(globals), m_elapsed_ms(0)
{
	(void)globals;
}

Clock::~Clock()
{

}

void    Clock::setGlobals(Globals& globals)
{
	(void)globals;
}

void   Clock::start()
{

}

void    Clock::update()
{
	
}

inline
int Clock::mf_get_time()
{
	return (1);
}


const char* Clock::get_FormatedTime()
{
	return (m_buffer);
}
