/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagerPrepareWorkers.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:04:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/03 09:19:45 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerManager.hpp"
# include "../ServerConfig/ServerConfig.hpp"

void	ServerManager::prepareWorkers()
{
	size_t maxConn;

	maxConn = m_config.getMaxConnections();
	m_workers.reserve(maxConn);
	for (size_t i = 0; i < maxConn; i++)
	{
		m_workers.emplace_back(*this, i, m_globals);
	}
}