/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/19 14:27:21 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP

# include "../GenericUtils/Webserver_Definitions.h"
# include "CgiManager/CgiManager.hpp"
# include "ListeningSocket/ListeningSocket.hpp"
# include "Connection/ConnectionPool.hpp"
# include "../Globals/LogFile/LogFile.hpp"
# include "../EventManager/EventManager.hpp"
# include "../Event/HandlerFunction.hpp"
# include "../SignalHandler/SignalHandler.hpp"

# include <vector>
# include <map>
# include <iostream>

class Globals;

class ServerManager
{
    public:
        ServerManager(size_t serverID, Globals* globals);
        ~ServerManager();

        int createListeners(const char* node, const char* port, int socktype, int ai_family, int backlog);

        int setup_mySignalHandler();

        int run();

    //private:
        //typedef MemoryPool_Alloc<std::pair<int, ListeningSocket> > ListeningSocketPool;
        //typedef std::map<int, ListeningSocket, std::less<int>, MemoryPool_Alloc<std::pair<int, ListeningSocket> > > ListeningSocketMap;
        //ListeningSocketMap              m_listeners;

        size_t                          m_myID;
        bool                            m_multithreadListen;
        std::vector<ListeningSocket*>   m_listeners;
        Nginx_MemoryPool*               m_pool;
        CgiManager                     	m_cgiHandler;
        ConnectionPool                  m_connectionPool;
        EventManager                    m_eventManager;
        Globals*                        m_globals;
        Event                           m_mySignalEvent;

        bool                            m_isRunning;



    private:
        ServerManager();
        ServerManager(const ServerManager& copy);
        ServerManager& operator=(const ServerManager& assign);
};





#endif
