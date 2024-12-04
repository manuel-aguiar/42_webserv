/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManagerPrepareWorkers.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:04:31 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/04 11:51:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Project headers
# include "ServerManager.hpp"
# include "../ServerConfig/ServerConfig.hpp"

// C++ headers



void	ServerManager::prepareWorkers()
{
	Nginx_MemoryPool*	workerMemPool;
	ServerWorker*		serverWorker;
	size_t				listenerCount;

	m_workers.reserve(m_config.getMaxWorkers());
	for (size_t i = 0; i < m_workers.size(); i++)
	{
		workerMemPool = Nginx_MemoryPool::create(4096, 1);
		serverWorker = (ServerWorker*)workerMemPool->allocate(sizeof (ServerWorker));
		new (serverWorker) ServerWorker(*this, i, workerMemPool, m_globals);
	}
}

