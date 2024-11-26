/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorkerExitHandler.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:59:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/26 09:00:31 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerWorker.hpp"
# include "../../Event/Event.hpp"

void ServerWorker::EventExit(Event& event)
{
    ServerWorker* server;

    server = reinterpret_cast<ServerWorker*>(event.m_data);
    server->m_isRunning = false;
}