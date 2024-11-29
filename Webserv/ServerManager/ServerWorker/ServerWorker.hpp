/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/29 10:08:21 by manuel           ###   ########.fr       */
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

        int 							run();

        int 							createListeners(const char* node, const char* port, int socktype, int ai_family, int backlog);
        int 							setup_mySignalHandler();

        static void 					EventExit(Event& event);


	// non-const accessors
		ServerManager&					accessServerManager();
		CgiManager&						accessCgiManager();
		ConnectionManager&				accessConnManager();
		EventManager&					accessEventManager();

	// getters
		const ServerManager&			getServerManager() const;
		const CgiManager&				getCgiManager() const;
		const ConnectionManager&		getConnManager() const;
		const EventManager&				getEventManager() const;


		bool							isRunning() const;

    private:

        size_t                          m_myID;
        std::vector<ListeningSocket*>   m_listeners;
        Nginx_MemoryPool*               m_pool;

		ServerManager&					m_serverManager;
        CgiManager                     	m_cgiManager;
        ConnectionManager               m_connManager;
        EventManager                    m_eventManager;

        Globals*                        m_globals;
        Event                           m_mySignalEvent;

		const ServerConfig&				m_config;

        bool                            m_isRunning;



        ServerWorker();
        ServerWorker(const ServerWorker& copy);
        ServerWorker& operator=(const ServerWorker& assign);
};





#endif
