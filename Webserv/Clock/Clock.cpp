/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clock.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:52:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/16 15:44:46 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clock.hpp"
#include "../Globals/Globals.hpp"


Clock::Clock(Globals* globals) : m_globals(globals), m_elapsed_ms(0)
{
	std::memset(m_buffer, 0, sizeof(m_buffer));
}

Clock::~Clock()
{

}

void    Clock::setGlobals(Globals& globals)
{
	m_globals = &globals;
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
	m_elapsed_ms = (m_now.tv_sec - m_start.tv_sec) * 1000 + (m_now.tv_usec - m_start.tv_usec) / 1000;
}

inline
int Clock::_get_time()
{
	if (::gettimeofday(&m_now, NULL) == -1)
	{
		m_globals->logStatus("gettimeofday(): " + std::string(std::strerror(errno)));
		return (0);
	}
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
	struct tm*   timeinfo;
	char*        bufPosi;

	timeinfo = std::localtime(&m_now.tv_sec);
	if (timeinfo == NULL)
	{
		m_globals->logStatus("localtime(): " + std::string(std::strerror(errno)));
		return (NULL);
	}
	bufPosi = m_buffer;
	bufPosi = inPlacePutDateParameter(timeinfo->tm_year + 1900, bufPosi, 4);
	*bufPosi++ = '-';
	bufPosi = inPlacePutDateParameter(timeinfo->tm_mon + 1, bufPosi, 2);
	*bufPosi++ = '-';
	bufPosi = inPlacePutDateParameter(timeinfo->tm_mday, bufPosi, 2);
	*bufPosi++ = '_';
	*bufPosi++ = '_';
	bufPosi = inPlacePutDateParameter(timeinfo->tm_hour, bufPosi, 2);
	*bufPosi++ = ':';
	bufPosi = inPlacePutDateParameter(timeinfo->tm_min, bufPosi, 2);
	*bufPosi++ = ':';
	bufPosi = inPlacePutDateParameter(timeinfo->tm_sec, bufPosi, 2);
	*bufPosi++ = '_';
	*bufPosi++ = '_';
	bufPosi = inPlacePutDateParameter(m_now.tv_usec, bufPosi, 6);
	*bufPosi++ = '\0';

	/*
		ooooorrrr......

		#include <cstdio>
		std::sprintf(_buffer, "%04d-%02d-%02d %02d:%02d:%02d:%06d",
			timeinfo->tm_year + 1900,
			timeinfo->tm_mon + 1,
			timeinfo->tm_mday,
			timeinfo->tm_hour,
			timeinfo->tm_min,
			timeinfo->tm_sec,
			m_now.tv_usec
		);

		hey, dunno, subject requirements... "no C stuff"...?
	*/

	return (m_buffer);
}
