/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 09:53:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/17 17:27:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"
# include <cassert>
# include <cstddef>

Event::Event() :
	m_fd				(-1),
	m_monitoredFlags	(0),
	m_triggeredFlags	(0),
	m_subscribedFd		(-1),
	m_subscribedFlags	(-1)
{}

Event::~Event() {}


void Event::handle()
{
	m_callback.execute();
}

void	Event::reset()
{
	m_fd = -1;
	m_monitoredFlags = 0;
	m_triggeredFlags = 0;
	m_callback.reset();
}

//private
Event::Event(const Event& copy)  {(void)copy;}
Event& Event::operator=(const Event& assign) {(void)assign; return (*this);}
