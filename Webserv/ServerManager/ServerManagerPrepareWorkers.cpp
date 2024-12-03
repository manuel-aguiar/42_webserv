/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagerPrepareWorkers.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:04:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/03 10:01:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "ServerManager.hpp"
# include "../ServerConfig/ServerConfig.hpp"

// C++ headers

static size_t countListeners(const ServerConfig& config)
{
	typedef std::map<std::string, ServerBlocks> t_blocks;

	size_t count = 0;
	const t_blocks& blocks = config.getServerBlocks();

	for (t_blocks::const_iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
		count += iter->second.getListeners().size();
	
	return (count);
}

void	ServerManager::prepareWorkers()
{
	size_t	maxConn;
	size_t	listenerCount;

	maxConn = m_config.getMaxConnections();
	listenerCount = countListeners(m_config);
	m_workers.reserve(maxConn);
	for (size_t i = 0; i < maxConn; i++)
	{
		m_workers.emplace_back(*this, i, m_globals);
		m_workers.back().accessListeners().reserve(listenerCount);
	}
}

