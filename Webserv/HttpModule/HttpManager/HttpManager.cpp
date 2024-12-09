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

#include "HttpManager.hpp"

HttpManager::HttpManager(ServerManager& serverManager) :
	m_serverManager(serverManager)
{
}

HttpManager::~HttpManager() {}

HttpManager::HttpManager(const HttpManager& copy) :
	m_serverManager(copy.m_serverManager)
{
}