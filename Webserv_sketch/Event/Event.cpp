/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 11:00:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"
# include "../Connection/Connection.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"

Event::Event(Globals* globals) : _globals(globals)
{
    
}


Event::~Event()
{
    
}

void Event::setHandler(EventHandler handler)
{
    _handler = handler;
}

void Event::handle()
{
    assert(this->_handler != NULL);
    
    (this->_handler)(*this);
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