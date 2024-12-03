/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocketEvents.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 09:03:06 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/29 17:03:17 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ListeningSocket.hpp"
# include "../../Event/Event.hpp"

void ListeningSocket::EventAccept(Event& event)
{
	ListeningSocket*	listener;

	listener = reinterpret_cast<ListeningSocket*>(event.getData());
	listener->accept();
}
