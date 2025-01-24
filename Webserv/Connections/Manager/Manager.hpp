

# ifndef CONNECTIONMANAGER_HPP

# define CONNECTIONMANAGER_HPP

//  Project Headers
# include "../../../GenericUtils/Webserver_Definitions.h"
# include "../../../Globals/LogFile/LogFile.hpp"
# include "../../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

class Connection;

namespace Conn
{
	class Manager
	{
		public:

			class Connection;

			Manager(size_t maxConnections, Globals& globals);
			~Manager();

			Connection*				provideConnection();
			void					returnConnection(Connection& connection);

		private:

			class InternalConn;

			size_t								m_maxConnections;
			HeapArray<InternalConn>				m_connections;
			HeapCircularQueue<InternalConn*>	m_spareConnections;

			Globals&							m_globals;


			void 					mf_destroyConnection(Connection* connection);

			Manager(const Manager& copy);
			Manager& operator=(const Manager& assign);
	};
}



# endif
