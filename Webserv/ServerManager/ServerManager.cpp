/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:56:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/18 14:48:01 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerManager.hpp"
# include "../GenericUtils/Webserver_Definitions.h"
# include "../HttpModule/HttpModule.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../Globals/SignalHandler/SignalHandler.hpp"

ServerManager::ServerManager(const ServerConfig& config, Globals* globals) :
	m_blockFinder(config),
	m_config(config),
	m_globals(globals)
{
	m_protoModules[HTTP_MODULE] = new HttpModule(*this);
}

ServerManager::~ServerManager()
{
	for (size_t i = 0; i < MODULE_COUNT; ++i)
		delete ((unsigned char *)m_protoModules[i]);
}

void    ServerManager::run()
{
	if (m_workers.size() == 1)
		mf_runSingleThreaded();
	else
		mf_runMultiThreaded();

	// destroy workers,
	// close signal handler pipes
}

void    ServerManager::mf_runSingleThreaded()
{
	m_workers[0]->run();
}

void    ServerManager::mf_runMultiThreaded()
{
	m_threadPool = new ThreadPool(m_workers.size());

	for (size_t i = 0; i < m_workers.size(); ++i)
	{
		m_threadPool->addTask(*m_workers[i], &ServerWorker::run);
	}

	while (!SignalHandler::getSignal())
		usleep(1000);

	m_threadPool->destroy(true);
	delete (m_threadPool);
}
