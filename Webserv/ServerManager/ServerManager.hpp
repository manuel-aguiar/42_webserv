/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:44:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/02 10:41:51 by mmaria-d         ###   ########.fr       */
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
		~ServerManager();

		void*							accessProtoModule(e_protoModules module) const;
		const ServerConfig& 			getConfig() const;

		void							prepareWorkers();
		void							run();


	private:

		std::vector<ServerWorker>		m_workers;
		BlockFinder						m_blockFinder;
		const ServerConfig&				m_config;
		Globals*						m_globals;

		t_ptr_ProtoModule				m_protoModules[MODULE_COUNT];	// loads the modules that we will be using

		ThreadPool*                     m_threadPool;

		void    						mf_runSingleThreaded();
		void    						mf_runMultiThreaded();

		ServerManager();
		ServerManager(const ServerManager& copy);
		ServerManager& operator=(const ServerManager& assign);
};



#endif
