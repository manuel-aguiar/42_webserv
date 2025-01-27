

# ifndef CONNECTIONIMPLMANAGER_HPP

# define CONNECTIONIMPLMANAGER_HPP

//  Project Headers
# include "../../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

// C++ headers
# include <vector>

// forward declarations
class InternalConn;
class ListeningSocket;
class ServerContext;
namespace Events { class Manager; }
namespace Ws { struct BindInfo; }
namespace Conn { class Connection; }

class ImplManager
{
	protected:
		ImplManager(const size_t maxConnections,
					const std::vector<Ws::BindInfo>& bindAddresses,
					Events::Manager& eventManager, 
					ServerContext& context);
		~ImplManager();
	
	public:

		// public interface
		void								init();
		void								shutdown();



		// helper methods that will actually be called internally between helpers
		// will be hidden via private inheritance for the public interface



		// access to the event manager
		Events::Manager&					accessEventManager();


		InternalConn*						_Listener_ProvideConnection();
		void								_Listener_ReturnConnection(InternalConn& connection);
		void								_Listener_MoveToPendingAccept(ListeningSocket& listener);

	private:
		size_t								m_maxConnections;
		HeapArray<InternalConn>				m_connections;
		HeapCircularQueue<InternalConn*>	m_spareConnections;

		HeapArray<ListeningSocket>			m_listeningSockets;
		HeapCircularQueue<ListeningSocket*>	m_pendingAccepts;

		Events::Manager&					m_eventManager;
		ServerContext&						m_context;

		ImplManager(const ImplManager& copy);
		ImplManager& operator=(const ImplManager& assign);
};

# endif
