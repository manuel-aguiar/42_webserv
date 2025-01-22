

# ifndef CONNECTIONMANAGER_HPP

# define CONNECTIONMANAGER_HPP

//  Project Headers
# include "../../Connection/Connection.hpp"
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Globals/LogFile/LogFile.hpp"
# include "../../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.hpp"
# include "../../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_PoolAllocator.hpp"
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"


// C++ headers
# include <queue>
# include <list>
# include <vector>

class ConnectionManager
{
	public:
		ConnectionManager(size_t maxConnections, Nginx_MemoryPool& borrowedPool, Globals& globals);
		~ConnectionManager();

		Connection*				provideConnection();
		void					returnConnection(Connection& connection);

	private:

		class ManagedConnection : public Connection
		{
			public:
				ManagedConnection(Globals& globals);
				~ManagedConnection();
				ManagedConnection(const ManagedConnection& copy);
				ManagedConnection& operator=(const ManagedConnection& assign);
		};

		size_t																		m_maxConnections;
		HeapArray<ManagedConnection, Nginx_PoolAllocator<ManagedConnection> >		m_connections;
		HeapArray<Subscription, Nginx_PoolAllocator<Subscription> >								m_readEvents;
		HeapArray<Subscription, Nginx_PoolAllocator<Subscription> >								m_writeEvents;
		HeapArray<ManagedConnection*, Nginx_PoolAllocator<ManagedConnection*> >		m_spareConnections;

		Globals&																	m_globals;


		void 					mf_destroyConnection(Connection* connection);

		ConnectionManager(const ConnectionManager& copy);
		ConnectionManager& operator=(const ConnectionManager& assign);
};




# endif
