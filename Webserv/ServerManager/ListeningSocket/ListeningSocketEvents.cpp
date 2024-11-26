/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocketEvents.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:03:06 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/26 09:23:43 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ListeningSocket.hpp"
# include "../../Event/Event.hpp"

void ListeningSocket::EventAccept(Event& event)
{
    ListeningSocket* listener;

    listener = reinterpret_cast<ListeningSocket*>(event.getData());
    listener->accept();
}