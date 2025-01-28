

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"

# include "../Socket/Socket.hpp"
# include "../Monitor/Monitor.hpp"
# include "../Listener/Listener.hpp"

//forward declarations
class ImplManager;
class InternalConn;
class Globals;
class ServerContext;

class ListeningSocket
{
	public:
		ListeningSocket(const int backlog, const Ws::BindInfo& info, ImplManager& connManager);
		~ListeningSocket();

		// methods
		int                         open();
		void						accept();
		void                        close();

		int							acceptPending(InternalConn& connection);

	private:
		Socket						m_socket;
		Monitor						m_monitor;
		Listener					m_listener;
		ImplManager&				m_connManager;

		ServerContext&				mf_accessServerContext();
		Globals&					mf_accessGlobals();
		int							mf_acceptInternal(InternalConn& connection);

		static void                 EventCallbackAccept(Events::Subscription& callback);

		ListeningSocket(const ListeningSocket& copy);
		ListeningSocket& operator=(const ListeningSocket& assign);

};


#endif
