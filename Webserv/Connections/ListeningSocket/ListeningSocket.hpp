

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"



//forward declarations
class Subscription;
class ServerWorker;
class Globals;
struct BindAddress;
namespace Conn (class Connection;)
namespace Events (class Subscription;)
namespace Events (class Manager;)

class ListeningSocket
{
	public:
		ListeningSocket(const int backlog, const BindAddress& info, Globals& globals);
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

		// setters
		void						setProtoModule					(const t_ptr_ProtoModule& module);
		void						setInitProtocolConnection		(const t_func_initProtoConn& func);

		// accessors
		Events::Subscription*		accessEvent();

	private:

		Ws::Sock::fd				m_sockfd;
		int							m_backlog;

		t_func_initProtoConn		m_initConnection;
		t_ptr_ProtoModule			m_protoModule;
		
		Events::Subscription*		m_event;
		const BindAddress&			m_info;
		Globals&                    m_globals;

		ListeningSocket();
		ListeningSocket(const ListeningSocket& copy);
		ListeningSocket& operator=(const ListeningSocket& assign);

		int    mf_close_accepted_connection(Connection& connection);
		int    mf_bind();
		int    mf_listen();

};


#endif
