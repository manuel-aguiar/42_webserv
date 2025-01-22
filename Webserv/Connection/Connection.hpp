

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

// Toolkit headers
# include "../../Toolkit/MemoryPool/Nginx_MemoryPool/Nginx_MemoryPool.hpp"

// Project headers
# include "../GenericUtils/Webserver_Definitions.h"

class Globals;
class ListeningSocket;
class Subscription;

class Connection
{
	protected:
		Connection(Globals& globals);
		~Connection();
		Connection(const Connection& other);
		Connection& operator=(const Connection& other);

	public:

		typedef void 			(*Init)(Connection*);
	//methods
		void    				init();
		void    				reset();
		void    				read();
		void    				write();
		void    				close();

	//getters
		t_socket				getSocket()			const;
		const t_sockaddr*		getAddr()			const;
		t_socklen				getAddrlen()		const;
		const Subscription&			getReadEvent()		const;
		const Subscription&			getWriteEvent()		const;
		const ListeningSocket&	getListener()		const;
		const Globals&			getGlobals()		const;
		const Nginx_MemoryPool&	getMemPool()		const;

	//setters
		void					setSocket			(const t_socket sockfd);
		void					setAddr				(t_sockaddr* addr);
		void					setAddrlen			(const t_socklen addrlen);
		void					setReadEvent		(Subscription& event);
		void					setWriteEvent		(Subscription& event);
		void					setListener			(ListeningSocket& listener);
		void					setProtoConnection	(const t_ptr_ProtoConnection connection);
		void					setProtoModule		(const t_ptr_ProtoModule module);


	//accessors
		t_sockaddr*				accessAddr();
		t_ptr_ProtoConnection	accessProtoConnection();
		t_ptr_ProtoModule		accessProtoModule();
		Nginx_MemoryPool&		accessMemPool();
		Subscription&					accessReadEvent();
		Subscription&					accessWriteEvent();




	private:
		// set by ListeningSocket that accepts
		t_socket				m_sockfd;
		t_sockaddr*				m_addr;
		t_socklen				m_addrlen;
		Subscription*					m_readEvent;
		Subscription*					m_writeEvent;
		ListeningSocket*		m_listener;
		t_ptr_ProtoConnection	m_ptr_protoConnection;				// <- the http connection
		t_ptr_ProtoModule		m_ptr_protoModule;					// <- the http module in our case

		// owned by the connection
		Nginx_MemoryPool&		m_memPool;
		Globals&				m_globals;
																				// set by the listening socket

};



#endif
