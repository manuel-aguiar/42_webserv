/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:44:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/28 16:50:53 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP

# define SERVERMANAGER_HPP

// Project Headers
# include "ServerWorker/ServerWorker.hpp"
# include "BlockFinder/BlockFinder.hpp"

// C++ headers
# include <vector>

class ServerConfig;
class Globals;
class ThreadPool;
class BlockFinder;

typedef enum e_protoModules
{
	HTTP_MODULE,
	MODULE_COUNT
} e_protoModules;


class ServerManager
{
	public:
		ServerManager(const ServerConfig& config, Globals* globals = NULL);


		void*	getProtoModule(e_protoModules module) const;
		void	prepareWorkers();
		void	run();

	private:

		std::vector<ServerWorker>		m_workers;
		BlockFinder						m_blockFinder;
		const ServerConfig&				m_config;
		Globals*						m_globals;

		void*							m_protoModules[MODULE_COUNT];	// loads the modules that we will be using

		ThreadPool*                     m_threadPool;

		void    mf_runSingleThreaded();
		void    mf_runMultiThreaded();
};



#endif
