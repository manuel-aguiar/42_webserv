

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"

//forward declarations
class ImplManager;
class InternalConn;
class Globals;
class ServerContext;
namespace Events { class Subscription; }
namespace Events { class Manager; }

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
		Ws::Sock::fd				m_sockfd;
		int							m_backlog;
		const Ws::BindInfo&			m_info;
		Events::Subscription*		m_eventSubs;
		ImplManager&				m_connManager;

		ServerContext&				mf_accessServerContext();
		Events::Manager&			mf_accessEventManager();
		Globals&					mf_accessGlobals();
		int							mf_acceptInternal(InternalConn& connection);

		static void                 EventCallbackAccept(Events::Subscription& callback);

		ListeningSocket(const ListeningSocket& copy);
		ListeningSocket& operator=(const ListeningSocket& assign);

};


#endif
