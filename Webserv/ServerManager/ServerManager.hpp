/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:44:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/22 10:47:52 by manuel           ###   ########.fr       */
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

class ServerManager
{
    public:
        ServerManager(const ServerConfig& config, Globals* globals = NULL);

		void	prepareWorkers();
        void    run();

    private:

        std::vector<ServerWorker>       m_workers;
		BlockFinder						m_blockFinder;
		const ServerConfig&             m_config;
        Globals*                        m_globals;


		ThreadPool*                     m_threadPool;

        void    mf_runSingleThreaded();
        void    mf_runMultiThreaded();
};



#endif
