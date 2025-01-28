

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"

# include "../Socket/Socket.hpp"
# include "../Monitor/Monitor.hpp"
# include "../Listener/Listener.hpp"

//forward declarations
class InternalManager;
class InternalConn;
class Globals;
class ServerContext;

class InternalListen
{
	public:
		InternalListen(const int backlog, const Ws::BindInfo& info, InternalManager& connManager);
		~InternalListen();

		// methods
		int                         open();
		void						accept();
		void                        close();

		int							acceptPending(InternalConn& connection);

	private:
		Socket						m_socket;
		Monitor						m_monitor;
		Listener					m_listener;
		InternalManager&			m_connManager;

		ServerContext&				mf_accessServerContext();
		Globals&					mf_accessGlobals();
		int							mf_acceptInternal(InternalConn& connection);

		static void                 EventCallbackAccept(Events::Subscription& callback);

		InternalListen(const InternalListen& copy);
		InternalListen& operator=(const InternalListen& assign);

};


#endif
