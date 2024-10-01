/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:17:27 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 08:04:15 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Event.hpp"
# include "../Connection/Connection.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"

Event::Event(ILog* logFile) : _logFile(logFile)
{
}


Event::~Event()
{
    
}

void Event::setHandler(void (Event::*handler)())
{
    _handler = handler;
}

void Event::handle()
{
    assert(this->_handler != NULL);
    (this->*_handler)();
}

void Event::accept()
{
    _connection->_listener->accept();
}

void Event::read()
{
    _connection->read();
}

void Event::write()
{
    _connection->write();
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