/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 10:45:07 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 11:14:29 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventHandler.hpp"
#include "Event.hpp"
#include "../Connection/Connection.hpp"
#include "../ListeningSocket/ListeningSocket.hpp"

void EventHandler::accept(Event& event)
{
    event._connection->_listener->accept();
}

void EventHandler::read(Event& event)
{
    (void)event;
}

void EventHandler::write(Event& event)
{
    (void)event;
}

void EventHandler::Cgi_write(Event& event)
{
    (void)event;
}

void EventHandler::Cgi_read(Event& event)
{
    (void)event;
}

void EventHandler::signalRead(Event& event)
{
    (void)event;
}
