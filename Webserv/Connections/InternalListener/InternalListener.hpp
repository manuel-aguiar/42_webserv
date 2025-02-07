

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../Ws_Namespace.h"

# include "../Socket/Socket.hpp"
# include "../Monitor/Monitor.hpp"
# include "../Listener/Listener.hpp"

//forward declarations
class InternalManager;
class InternalConn;
class Globals;
class ServerContext;
namespace Events { class Manager; }

class InternalListener
{
	public:
		InternalListener(const Ws::BindInfo& info, InternalManager& connManager);
		~InternalListener();

		// methods
		int                         open();
		void						accept();
		void                        close();

		int							acceptPending(InternalConn& connection);

	private:
		Monitor						m_monitor;
		Listener					m_listener;
		InternalManager&			m_connManager;

		Events::Manager&			mf_accessEventManager();
		ServerContext&				mf_accessServerContext();
		Globals&					mf_accessGlobals();
		int							mf_acceptInternal(InternalConn& connection);

		static void                 EventCallbackAccept(Events::Subscription& callback);

		InternalListener(const InternalListener& copy);
		InternalListener& operator=(const InternalListener& assign);

};


#endif
