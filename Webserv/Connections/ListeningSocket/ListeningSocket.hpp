

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"

//forward declarations
class ImplManager;
class Globals;
class ServerContext;
class InternalConn;
namespace Events {class Subscription;}
namespace Events {class Manager;}

class ListeningSocket
{
	public:
		ListeningSocket(const int backlog, const Ws::BindInfo& info, ImplManager& connManager, ServerContext& context);
		~ListeningSocket();

		// methods
		int                         open();
		int                        	accept();

		void                        close();
		void                        closeConnection(Conn::Connection& connection);

		// events callbacks
		static void                 EventCallbackAccept(Events::Subscription& callback);

		void						setEvent(Events::Subscription* event);
		Events::Subscription*		accessEvent();

	private:

		Ws::Sock::fd				m_sockfd;
		int							m_backlog;

		Ws::AppLayer::Init			m_init;
		Ws::AppLayer::Module		m_module;
		
		Events::Subscription*		m_event;

		const Ws::BindInfo&			m_info;
		ImplManager&				m_connManager;
		ServerContext&				m_context;

		ListeningSocket();
		ListeningSocket(const ListeningSocket& copy);
		ListeningSocket& operator=(const ListeningSocket& assign);

		int    mf_close_accepted_connection(Conn::Connection& connection);
		int    mf_bind();
		int    mf_listen();

};


#endif
