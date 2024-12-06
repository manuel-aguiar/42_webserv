/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:44:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 10:45:17 by manuel           ###   ########.fr       */
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

		//methods
		void							prepareWorkers();
		void							run();

		//getters
		const ServerConfig& 			getConfig() const;

		//accessors
		t_ptr_ProtoModule				accessProtoModule(e_protoModules module);


	private:
		DynArray<ServerWorker*>			m_workers;
		BlockFinder						m_blockFinder;
		const ServerConfig&				m_config;
		Globals*						m_globals;

		t_ptr_ProtoModule				m_protoModules[MODULE_COUNT];	// loads the modules that we will be using
		t_ptr_ProtoConnection			m_protoConnections[MODULE_COUNT];	// loads the connections that we will be using

		ThreadPool*                     m_threadPool;

		void    						mf_runSingleThreaded();
		void    						mf_runMultiThreaded();

		ServerManager();
		ServerManager(const ServerManager& copy);
		ServerManager& operator=(const ServerManager& assign);
};



#endif
