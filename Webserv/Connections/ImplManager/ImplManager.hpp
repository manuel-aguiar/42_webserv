

# ifndef CONNECTIONIMPLMANAGER_HPP

# define CONNECTIONIMPLMANAGER_HPP

//  Project Headers
# include "../../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

// C++ headers
# include <vector>

// forward declarations
class Connection;
class InternalConn;
class ListeningSocket;
class ServerContext;
namespace Events { class Manager; }
struct BindAddress;

class ImplManager
{
	protected:
		ImplManager(const size_t maxConnections,
					const std::vector<BindAddress>& bindAddresses,
					Events::Manager& eventManager, 
					ServerContext& context);
		~ImplManager();
	
	public:
		void								startListening();
		void								stopListening();

		Connection*							provideConnection();
		void								returnConnection(Connection& connection);

	private:
		size_t								m_maxConnections;
		HeapArray<InternalConn>				m_connections;
		HeapCircularQueue<InternalConn*>	m_spareConnections;

		HeapArray<ListeningSocket>			m_listeningSockets;
		Events::Manager&					m_eventManager;
		ServerContext&						m_globals;

		void 					mf_destroyConnection(Connection* connection);

		ImplManager(const ImplManager& copy);
		ImplManager& operator=(const ImplManager& assign);
};

# endif
