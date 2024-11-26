/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/26 09:11:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERWORKER_HPP

# define SERVERWORKER_HPP

// Project Headers
# include "../CgiManager/CgiManager.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"
# include "../ConnectionManager/ConnectionManager.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Globals/LogFile/LogFile.hpp"
# include "../../EventManager/EventManager.hpp"
# include "../../../Globals/SignalHandler/SignalHandler.hpp"

// C++ headers
# include <vector>
# include <map>
# include <iostream>

class Event;
class Globals;
class ServerManager;

class ServerWorker
{
    public:
        ServerWorker(ServerManager& manager, size_t serverID, Globals* globals);
        ~ServerWorker();

        int run();

        int createListeners(const char* node, const char* port, int socktype, int ai_family, int backlog);
        int setup_mySignalHandler();

        static void EventExit(Event& event);


        size_t                          m_myID;
        std::vector<ListeningSocket*>   m_listeners;
        Nginx_MemoryPool*               m_pool;
        CgiManager                     	m_cgiHandler;
        ConnectionManager               m_connectionPool;
        EventManager                    m_eventManager;

        Globals*                        m_globals;
        Event                           m_mySignalEvent;

		ServerManager&					m_serverManager;

        bool                            m_isRunning;



    private:
        ServerWorker();
        ServerWorker(const ServerWorker& copy);
        ServerWorker& operator=(const ServerWorker& assign);
};





#endif
