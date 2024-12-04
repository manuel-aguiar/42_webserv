/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorkerGetSetAccess.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 10:05:45 by manuel            #+#    #+#             */
/*   Updated: 2024/12/04 16:12:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerWorker.hpp"



// getters

int							ServerWorker::getID() const
{
	return (m_myID);
}

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

const Nginx_MemoryPool&		ServerWorker::getMemPool() const
{
	return (*m_memPool);
}


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

DynArray<ListeningSocket, Nginx_PoolAllocator<ListeningSocket> >&		ServerWorker::accessListeners()
{
	return (m_listeners);
}

Nginx_MemoryPool&			ServerWorker::accessMemPool()
{
	return (*m_memPool);
}