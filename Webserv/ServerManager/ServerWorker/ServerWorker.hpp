/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerWorker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:03:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/06 10:18:29 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERWORKER_HPP

# define SERVERWORKER_HPP

// Project Headers
# include "../GenericUtils/Webserver_Definitions.h"
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
		ServerWorker(ServerManager& manager, size_t serverID, Nginx_MemoryPool* memPool, Globals* globals);
		~ServerWorker();

		// methods
		int 							run();
		int 							createListeners(const char* node, const char* port, int socktype, int ai_family, int backlog);
		int 							setup_mySignalHandler();

		//event handlers
		static void 					EventExit(Event& event);


	// getters
		int								getID()				const;
		const ServerManager&			getServerManager()	const;
		const CgiManager&				getCgiManager()		const;
		const ConnectionManager&		getConnManager()	const;
		const EventManager&				getEventManager()	const;
		const DynArray<ListeningSocket*, Nginx_PoolAllocator<ListeningSocket*> >&
										getListeners()		const;
		const Nginx_MemoryPool&			getMemPool()		const;

		bool							isRunning()			const;

	// accessors
		ServerManager&					accessServerManager();
		CgiManager&						accessCgiManager();
		ConnectionManager&				accessConnManager();
		EventManager&					accessEventManager();

		DynArray<ListeningSocket*, Nginx_PoolAllocator<ListeningSocket*> >&
										accessListeners();
		Nginx_MemoryPool&				accessMemPool();



    private:

		size_t                          				m_myID;

		ServerManager&									m_serverManager;
		const ServerConfig&								m_config;
		ConnectionManager								m_connManager;
		EventManager									m_eventManager;
		CgiManager										m_cgiManager;
		Event											m_mySignalEvent;
		Nginx_MemoryPool*								m_memPool;
		DynArray<ListeningSocket*,
			Nginx_PoolAllocator<ListeningSocket*> >		m_listeners;
		bool											m_isRunning;
		Globals*										m_globals;


		ServerWorker();
		ServerWorker(const ServerWorker& copy);
		ServerWorker& operator=(const ServerWorker& assign);
};





#endif
