/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MockClock.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 09:03:09 by mmaria-d         ###   ########.fr       */
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


static char	*inPlacePutDateParameter(unsigned int nb, char* buffer, int totalCharacters)
{
	char	print;
	int		i;

	i = totalCharacters;

	while (nb != 0 && i >= 0)
	{
		print = nb % 10;
		buffer[--i] = print + '0';
		nb /= 10;
	}

	while (i > 0)
		buffer[--i] = '0';

	return (&buffer[totalCharacters]);
}

const char* Clock::get_FormatedTime()
{
	return (m_buffer);
}
