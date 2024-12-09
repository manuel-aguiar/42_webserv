/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpManager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:17:34 by codespace         #+#    #+#             */
/*   Updated: 2024/11/11 10:17:37 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Project headers
# include "HttpManager.hpp"

//C++ headers
# include <cassert>

HttpManager::HttpManager(ServerManager& serverManager) :
	m_serverManager(serverManager)
{
}


void	HttpManager::addConnection(HttpConnection& connection)
{
	m_connections.insert(&connection);
}

void	HttpManager::removeConnection(HttpConnection& connection)
{
	size_t res = m_connections.erase(&connection);
	assert(res != 0);	// don't remove if it doesn't exist
	(void)res;
}


//private
HttpManager::~HttpManager() {}

HttpManager::HttpManager(const HttpManager& copy) :
	m_serverManager(copy.m_serverManager)
{
}