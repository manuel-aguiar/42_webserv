/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorkerEvents.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:59:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/29 10:33:42 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerWorker.hpp"
# include "../../Event/Event.hpp"

void ServerWorker::EventExit(Event& event)
{
    ServerWorker* server;

    server = reinterpret_cast<ServerWorker*>(event.getData());
    server->m_isRunning = false;
}
