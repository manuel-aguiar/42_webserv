

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

// Toolkit headers
# include "../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.hpp"

// Project headers
# include "../Manager/Manager.hpp"
# include "../GenericUtils/Webserver_Definitions.h"
# include "../../Events/Events.h"

class Globals;
class ListeningSocket;

namespace Conn
{
	class Manager::Connection
	{
		protected:
			Connection(Globals& globals);
			~Connection();
			Connection(const Connection& other);
			Connection& operator=(const Connection& other);

		public:

			typedef void 				(*Init)(Connection*);

		//methods
			void    					init();
			void    					reset();
			void    					close();

		//getters
			const Ws::Sock::addr&		getAddr()			const;
			const Events::Subscription&	getEventSubs()		const;
			const ListeningSocket&		getListener()		const;
			const Globals&				getGlobals()		const;

		//setters
			void						setSocket			(const t_socket sockfd);
			void						setAddr				(const Ws::Sock::addr& addr);
			void						setAddrlen			(const t_socklen addrlen);
			void						setReadEvent		(Events::Subscription& event);
			void						setListener			(ListeningSocket& listener);
			void						setProtoConnection	(const t_ptr_ProtoConnection connection);
			void						setProtoModule		(const t_ptr_ProtoModule module);


		//accessors
			Ws::Sock::addr&				accessAddr();
			t_ptr_ProtoConnection		accessProtoConnection();
			t_ptr_ProtoModule			accessProtoModule();
			Events::Subscription&		accessReadEvent();
			Events::Subscription&		accessWriteEvent();


		private:
			// set by ListeningSocket that accepts
			Ws::Sock::fd				m_sockfd;
			Ws::Sock::addr				m_addr;
			Ws::Sock::addrlen			m_addrlen;
			Events::Subscription*		m_eventSubs;
			t_ptr_ProtoConnection		m_ptr_protoConnection;				// <- the http connection
			t_ptr_ProtoModule			m_ptr_protoModule;					// <- the http module in our case

			ListeningSocket*			m_listener;
			Globals&					m_globals;
																					// set by the listening socket

	};
}


#endif
