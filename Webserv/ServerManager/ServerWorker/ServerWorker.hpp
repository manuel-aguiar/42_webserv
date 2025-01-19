

#ifndef SERVERWORKER_HPP

# define SERVERWORKER_HPP

// Toolkit Headers
# include "../../../Toolkit/List/List.hpp"
# include "../../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MPool_FixedElem/Nginx_MPool_FixedElem.hpp"

// Project Headers
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
		ServerWorker(ServerManager& manager, size_t serverID, Nginx_MemoryPool& memPool, Globals& globals);
		~ServerWorker();

		// methods
		int 							run();

		int								prepareLaunch();

		// helpers
		void							addPendingAccept(ListeningSocket& listener);	
		void							createListeningSocket(const t_addrinfo& addrinfo, 
															const int backlog, 
															const t_ptr_ProtoModule& protoModule, 
															const t_func_initProtoConn& initProtoConnection);

		// interaction with ConnectionManager
		Connection*						provideConnection();
		void							returnConnection(Connection& connection);

		//event handlers
		static void 					EventCallbackExit(Callback& callback);


	// getters
		int								getID()				const;
		const ServerManager&			getServerManager()	const;
		const ConnectionManager&		getConnManager()	const;
		const EventManager&				getEventManager()	const;
		const DynArray<ListeningSocket*, Nginx_PoolAllocator<ListeningSocket*> >&
										getListeners()		const;
		const Nginx_MemoryPool&			getMemPool()		const;

		bool							isRunning()			const;
		void							stop();
	// accessors
		ServerManager&					accessServerManager();
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
		Event											m_mySignalEvent;
		Nginx_MemoryPool&								m_memPool;
		DynArray<ListeningSocket*,
			Nginx_PoolAllocator<ListeningSocket*> >		m_listeners;
		List<ListeningSocket*,
			Nginx_MPool_FixedElem<ListeningSocket*> >	m_pendingAccept;
		bool											m_isRunning;
		Globals&										m_globals;


		ServerWorker();
		ServerWorker(const ServerWorker& copy);
		ServerWorker& operator=(const ServerWorker& assign);
};



#endif
