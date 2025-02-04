

# ifndef CONNECTIONINTERNALMANAGER_HPP

# define CONNECTIONINTERNALMANAGER_HPP

//  Project Headers
# include "../../../Toolkit/Arrays/HeapArray/HeapArray.hpp"
# include "../../../Toolkit/Arrays/HeapCircularQueue/HeapCircularQueue.hpp"

// C++ headers
# include <vector>

// forward declarations
class InternalConn;
class Globals;
class ListeningSocket;
class ServerContext;

namespace Events { class Manager; }
namespace Ws { struct BindInfo; }
namespace Conn { class Connection; }

class InternalManager
{
	protected:
		InternalManager(const size_t maxConnections,
						const std::vector<Ws::BindInfo>& bindAddresses,
						Events::Manager& eventManager,
						Globals& globals,
						ServerContext& context);
		~InternalManager();

	public:

		// public interface
		bool								init();
		void								shutdown();

		// helper methods that will actually be called internally between helpers
		// will be hidden via private inheritance for the public interface

		InternalConn*						_Accepter_ProvideConnection();
		void								_Accepter_MoveToPendingAccept(ListeningSocket& listener);
		void								_ReturnConnection(Conn::Connection& connection);
		Events::Manager&					_accessEventManager();
		Globals&							_accessGlobals();
		ServerContext&						_accessServerContext();

	private:
		size_t								m_maxConnections;
		HeapArray<InternalConn>				m_connections;
		HeapCircularQueue<InternalConn*>	m_spareConnections;

		HeapArray<ListeningSocket>			m_listeningSockets;
		HeapCircularQueue<ListeningSocket*>	m_pendingAccepts;

		Events::Manager&					m_eventManager;
		Globals&							m_globals;
		ServerContext&						m_context;

		InternalManager(const InternalManager& copy);
		InternalManager& operator=(const InternalManager& assign);

};

# endif
