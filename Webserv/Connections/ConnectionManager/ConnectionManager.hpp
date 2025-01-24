

# ifndef CONNECTIONMANAGER_HPP

# define CONNECTIONMANAGER_HPP

//  Project Headers
# include "../../../GenericUtils/Webserver_Definitions.h"
# include "../../../Globals/LogFile/LogFile.hpp"
# include "../../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

class Connection;

class ConnectionManager
{
	public:
		ConnectionManager(size_t maxConnections, Globals& globals);
		~ConnectionManager();

		Connection*				provideConnection();
		void					returnConnection(Connection& connection);

	private:

		class InternalConn;

		size_t								m_maxConnections;
		HeapArray<InternalConn>				m_connections;
		HeapCircularQueue<InternalConn*>	m_spareConnections;

		Globals&							m_globals;


		void 					mf_destroyConnection(Connection* connection);

		ConnectionManager(const ConnectionManager& copy);
		ConnectionManager& operator=(const ConnectionManager& assign);
};




# endif
