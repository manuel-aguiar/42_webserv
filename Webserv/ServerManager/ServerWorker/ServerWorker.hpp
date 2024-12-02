/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/02 10:50:32 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERWORKER_HPP

# define SERVERWORKER_HPP

// Project Headers
# include "../CgiManager/CgiManager.hpp"
# include "../ListeningSocket/ListeningSocket.hpp"
# include "../EventManager/EventManager.hpp"
# include "../ConnectionManager/ConnectionManager.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Globals/Globals.hpp"

// C++ headers
# include <vector>
# include <map>
# include <iostream>

class Event;
class Globals;
class ServerManager;
class ServerConfig;

class ServerWorker
{
    public:
        ServerWorker(ServerManager& manager, size_t serverID, Globals* globals);
        ~ServerWorker();

	// methods
        int 							run();
        int 							createListeners(const char* node, const char* port, int socktype, int ai_family, int backlog);
        int 							setup_mySignalHandler();

	//event handlers
        static void 					EventExit(Event& event);


	// getters
		const ServerManager&			getServerManager() const;
		const CgiManager&				getCgiManager() const;
		const ConnectionManager&		getConnManager() const;
		const EventManager&				getEventManager() const;


	// accessors
		ServerManager&					accessServerManager();
		CgiManager&						accessCgiManager();
		ConnectionManager&				accessConnManager();
		EventManager&					accessEventManager();

		bool							isRunning() const;

    private:

        size_t                          m_myID;
        std::vector<ListeningSocket*>   m_listeners;

		ServerManager&					m_serverManager;
        CgiManager                     	m_cgiManager;
        ConnectionManager               m_connManager;
        EventManager                    m_eventManager;
		const ServerConfig&				m_config;

        Globals*                        m_globals;
        Event                           m_mySignalEvent;

        Nginx_MemoryPool*               m_memPool;

        bool                            m_isRunning;



        ServerWorker();
        ServerWorker(const ServerWorker& copy);
        ServerWorker& operator=(const ServerWorker& assign);
};





#endif
