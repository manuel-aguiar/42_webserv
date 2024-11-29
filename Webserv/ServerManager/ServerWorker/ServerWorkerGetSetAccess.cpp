/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorker_GetSetAccess.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 10:05:45 by manuel            #+#    #+#             */
/*   Updated: 2024/11/29 10:08:58 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerWorker.hpp"


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
