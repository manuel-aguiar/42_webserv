/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorkerEvents.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:59:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/20 12:22:17 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerWorker.hpp"
# include "../../Event/Event.hpp"

void ServerWorker::EventCallbackExit(Callback& event)
{
    ServerWorker* server;

    server = reinterpret_cast<ServerWorker*>(event.getData());
    server->stop();
}
