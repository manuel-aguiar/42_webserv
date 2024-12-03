/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorkerGetSetAccess.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 10:05:45 by manuel            #+#    #+#             */
/*   Updated: 2024/12/03 10:00:03 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerWorker.hpp"

//accessors
ServerManager&				ServerWorker::accessServerManager()
{
	return (m_serverManager);
}

CgiManager&					ServerWorker::accessCgiManager()
{
	return (m_cgiManager);
}

ConnectionManager&			ServerWorker::accessConnManager()
{
	return (m_connManager);
}

EventManager&				ServerWorker::accessEventManager()
{
	return (m_eventManager);
}

DynArray<ListeningSocket>&		ServerWorker::accessListeners()
{
	return (m_listeners);
}

// getters

const ServerManager&		ServerWorker::getServerManager() const
{
	return (m_serverManager);
}

const CgiManager&			ServerWorker::getCgiManager() const
{
	return (m_cgiManager);
}

const ConnectionManager&	ServerWorker::getConnManager() const
{
	return (m_connManager);
}

const EventManager&			ServerWorker::getEventManager() const
{
	return (m_eventManager);
}
