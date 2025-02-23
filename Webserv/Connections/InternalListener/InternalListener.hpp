

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../Ws_Namespace.h"

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
		InternalListener(const InternalListener& copy);
		~InternalListener();

		// methods
		int                         open();
		void						accept();
		void                        close();

		int							acceptPending(InternalConn& connection);

	private:
		Listener					m_listener;
		Monitor						m_monitor;
		InternalManager&			m_connManager;

		Events::Manager&			mf_accessEventManager();
		ServerContext&				mf_accessServerContext();
		Globals&					mf_accessGlobals();
		int							mf_acceptInternal(InternalConn& connection);

		static void                 EventCallbackAccept(Events::Subscription& callback);

		// private
		InternalListener& operator=(const InternalListener& assign);

};

#endif
