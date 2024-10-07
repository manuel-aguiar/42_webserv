/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 13:32:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"
# include "../Connection/Connection.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"

Event::Event(Globals* globals) //: _globals(globals)
{
    (void)globals;
}


Event::~Event()
{
    
}

void    Event::setFlags(int flags)
{
    _flags = flags;
}

void Event::setHandler_Function_and_Data(HandlerFunction handler, HandlerData data)
{
    _function = handler;
    _data = data;
}


void Event::handle()
{
    assert(this->_function != NULL);
    
    (this->_function)(*this);
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