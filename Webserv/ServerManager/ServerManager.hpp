/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:44:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/21 12:31:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP

# define SERVERMANAGER_HPP

// Project Headers
# include "ServerWorker/ServerWorker.hpp"

// C++ headers
# include <vector>

class ServerConfig;
class Globals;
class ThreadPool;

class ServerManager
{
    public:
        ServerManager(const ServerConfig& config, Globals* globals = NULL);

        void    run();

    private:

        std::vector<ServerWorker>       m_workers;
        ThreadPool*                     m_threadPool;              
        Globals*                        m_globals;


        void    mf_runSingleThreaded();
        void    mf_runMultiThreaded();
};



#endif