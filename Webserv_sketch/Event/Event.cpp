/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 09:53:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:11:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"
# include "../Connection/Connection.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"

Event::Event(Globals* globals) //: m_globals(globals)
{
    (void)globals;
}


Event::~Event()
{

}

void    Event::setFlags(int flags)
{
    m_flags = flags;
}

void Event::setHandler_Function_and_Data(HandlerFunction handler, HandlerData data)
{
    m_function = handler;
    m_data = data;
}


void Event::handle()
{
    assert(this->m_function != NULL);

    (this->m_function)(*this);
}


//private
Event::Event(const Event& copy)
{
    (void)copy;
}

Event& Event::operator=(const Event& assign)
{
    (void)assign;
    return (*this);
}
