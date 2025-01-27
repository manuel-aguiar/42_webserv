

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"



//forward declarations
class Subscription;
class ServerWorker;
namespace Conn {class Manager;}
namespace Conn {class Connection;}
namespace Events {class Subscription;}
namespace Events {class Manager;}

class ListeningSocket
{
	public:
		ListeningSocket(const int backlog, const Ws::BindInfo& info, Conn::Manager& globals);
		~ListeningSocket();

		// methods
		int                         open();
		int                        	accept();

		void                        close();
		void                        closeConnection(Conn::Connection& connection);

		// events callbacks
		static void                 EventCallbackAccept(Events::Subscription& callback);


		// getters
		const Events::Subscription*	getEvent()						const;

		// accessors
		Events::Subscription*		accessEvent();

	private:

		Ws::Sock::fd				m_sockfd;
		int							m_backlog;

		Ws::AppLayer::Init			m_initConnection;
		Ws::AppLayer::Module			m_protoModule;
		
		Events::Subscription*		m_event;
		const Ws::BindInfo&			m_info;
		Conn::Manager&				m_connManager;
		Globals&                    m_globals;

		ListeningSocket();
		ListeningSocket(const ListeningSocket& copy);
		ListeningSocket& operator=(const ListeningSocket& assign);

		int    mf_close_accepted_connection(Connection& connection);
		int    mf_bind();
		int    mf_listen();

};


#endif
