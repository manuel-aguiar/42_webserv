/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 09:53:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/16 09:19:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"
# include <cassert>
# include <cstddef>

Event::Event() :
	m_fd		(-1),
	m_data		(NULL),
	m_handler	(NULL),
	m_flags		(0)
{}

Event::~Event() {}


void Event::handle()
{
	assert(this->m_handler != NULL);

	(this->m_handler)(*this);
}

void	Event::reset()
{
	this->m_fd = -1;
	this->m_data = NULL;
	this->m_handler = NULL;
	this->m_flags = 0;
}

//private
Event::Event(const Event& copy)  {(void)copy;}
Event& Event::operator=(const Event& assign) {(void)assign; return (*this);}
