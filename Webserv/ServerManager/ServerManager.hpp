/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:44:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/18 14:47:36 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP

# define SERVERMANAGER_HPP

// Project Headers
# include "../GenericUtils/Webserver_Definitions.h"
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

		//getters
		const ServerConfig& 			getConfig() const;

		//accessors
		t_ptr_ProtoModule				accessProtoModule(e_protoModules module);

		void							prepareWorkers();
		void							run();
		void							setupListeners();
		void							setupSingleListener(const t_ip_str& ip, const t_port_str& port, int socktype, int addrFamily, int backlog);

	private:
		DynArray<ServerWorker*>			m_workers;
		DynArray<t_sockaddr>			m_listenAddrs;
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
