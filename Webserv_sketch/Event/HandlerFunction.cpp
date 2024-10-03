/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HandlerFunction.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 10:45:07 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 12:20:52 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HandlerFunction.hpp"
#include "Event.hpp"
#include "../Connection/Connection.hpp"
#include "../ListeningSocket/ListeningSocket.hpp"
#include "../Server/Server.hpp"

void HandlerFunction::listener_Accept(Event& event)
{
    ListeningSocket* listener;

    listener = reinterpret_cast<ListeningSocket*>(event._data);
    listener->accept();
}

void HandlerFunction::connection_Read(Event& event)
{
    (void)event;
}

void HandlerFunction::connection_Write(Event& event)
{
    (void)event;
}

void HandlerFunction::Cgi_Read(Event& event)
{
    (void)event;
}

void HandlerFunction::Cgi_Write(Event& event)
{
    (void)event;
}

void HandlerFunction::signal_Read(Event& event)
{
    Server* server;

    server = reinterpret_cast<Server*>(event._data);
    server->_isRunning = false;
}
