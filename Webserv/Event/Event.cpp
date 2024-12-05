/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 09:53:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/02 11:53:56 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"
# include <cassert>
# include <cstddef>

Event::Event() :
	m_function(NULL),
	m_data(NULL)
{}

Event::~Event() {}

void    Event::setFlags(int flags)
{
	m_flags = flags;
}

void Event::setHandlerFunction_and_Data(HandlerFunction handler, HandlerData data)
{
	m_function = handler;
	m_data = data;
}

void Event::handle()
{
	assert(this->m_function != NULL && this->m_data != NULL);

	(this->m_function)(*this);
}

//private
Event::Event(const Event& copy)  {(void)copy;}
Event& Event::operator=(const Event& assign) {(void)assign; return (*this);}
