/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:56:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/13 10:00:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "ServerManager.hpp"
# include "../ServerConfig/ServerConfig/ServerConfig.hpp"
# include "../GenericUtils/Webserver_Definitions.h"
# include "../HttpModule/HttpModule.hpp"
# include "../CgiModule/CgiModule.hpp"
# include "../Globals/SignalHandler/SignalHandler.hpp"

ServerManager::ServerManager(const ServerConfig& config, Globals& globals) :
	m_blockFinder(config),
	m_config(config),
	m_globals(globals)
{
	m_protoModules[HTTP_MODULE] = new HttpModule(*this);
	m_initProtoConnection[HTTP_MODULE] = &HttpModule::initConnection;
	mf_prepareWorkers();

	//prepare signal handler
	g_SignalHandler.prepare_signal(SignalHandler::signal_handler, m_config.getNumWorkers(), globals);
}

/*
	ServerWorkers are built inside the memory pool which doesn't call destructors
	We must call the destructors ourselves
*/
ServerManager::~ServerManager()
{
	for (size_t i = 0; i < MODULE_COUNT; ++i)
		delete ((unsigned char *)m_protoModules[i]);
	for (size_t i = 0; i < m_workers.size(); ++i)
		m_workers[i]->~ServerWorker();
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

/*
	Run ServerManager in a multi-threaded environment (1 worker per thread)

	Sets up signal handling such that only the main thread receives process group signals
	Opens the Threadpool that creates the threads with this signal mask
	Reinstate the previous mask

	Run.
*/
void    ServerManager::mf_runMultiThreaded()
{
	sigset_t threadSigSet;

	/*** UNPROTECTED ***/

	sigemptyset(&threadSigSet);
	sigaddset(&threadSigSet, SIGINT);
	sigaddset(&threadSigSet, SIGQUIT);
	pthread_sigmask(SIG_BLOCK , &threadSigSet, NULL);

	m_threadPool = new ThreadPool(m_workers.size());

	pthread_sigmask(SIG_UNBLOCK, &threadSigSet, NULL);

	for (size_t i = 0; i < m_workers.size(); ++i)
		m_threadPool->addTask(*m_workers[i], &ServerWorker::run);

	while (!g_SignalHandler.getSignal())
		usleep(1000);

	m_threadPool->destroy(true);
	delete (m_threadPool);
}
