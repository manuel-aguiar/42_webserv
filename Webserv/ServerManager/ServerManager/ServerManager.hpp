

#ifndef SERVERMANAGER_HPP

# define SERVERMANAGER_HPP

// Project Headers
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../ServerWorker/ServerWorker.hpp"
# include "../BlockFinder/BlockFinder.hpp"

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
		ServerManager(const ServerConfig& config, Globals& globals);
		~ServerManager();

		//methods
		void							run();

		//getters
		const ServerConfig& 			getConfig() 		const;
		size_t							getListenerCount()	const;

		const t_ptr_ProtoModule*		getProtoModules()	const;
		const t_func_initProtoConn*		getInitProtoConnections()	const;

		//accessors
		t_ptr_ProtoModule				accessProtoModule(e_protoModules module);


	private:
		HeapArray<ServerWorker*>		m_workers;
		BlockFinder						m_blockFinder;
		const ServerConfig&				m_config;
		size_t							m_listenerCount;
		Globals&						m_globals;

		t_ptr_ProtoModule				m_protoModules[MODULE_COUNT];	// loads the modules that we will be using
		t_func_initProtoConn			m_initProtoConnection[MODULE_COUNT];	// loads the connections that we will be using

		ThreadPool*                     m_threadPool;

		void							mf_prepareWorkers();
		void							mf_runSingleThreaded();
		void							mf_runMultiThreaded();

		ServerManager();
		ServerManager(const ServerManager& copy);
		ServerManager& operator=(const ServerManager& assign);
};



#endif
