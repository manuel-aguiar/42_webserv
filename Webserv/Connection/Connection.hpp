/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 08:58:43 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../GenericUtils/Webserver_Definitions.h"

class Globals;
class ListeningSocket;
class Event;

class Connection
{
	protected:
		Connection(Globals& globals);
		~Connection();
		Connection(const Connection& other);
		Connection& operator=(const Connection& other);

	public:
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
		const Event&			getReadEvent()		const;
		const Event&			getWriteEvent()		const;
		const ListeningSocket&	getListener()		const;
		const Globals&			getGlobals()		const;
		const Nginx_MemoryPool&	getMemPool()		const;

	//setters
		void					setSocket			(const t_socket sockfd);
		void					setAddr				(t_sockaddr* addr);
		void					setAddrlen			(const t_socklen addrlen);
		void					setReadEvent		(Event& event);
		void					setWriteEvent		(Event& event);
		void					setListener			(ListeningSocket& listener);
		void					setProtoConnection	(const t_ptr_ProtoConnection connection);
		void					setProtoModule		(const t_ptr_ProtoModule module);


	//accessors
		t_sockaddr*				accessAddr();
		t_ptr_ProtoConnection	accessProtoConnection();
		t_ptr_ProtoModule		accessProtoModule();
		Nginx_MemoryPool&		accessMemPool();
		Event&					accessReadEvent();
		Event&					accessWriteEvent();




	private:
		// set by ListeningSocket that accepts
		t_socket				m_sockfd;
		t_sockaddr*				m_addr;
		t_socklen				m_addrlen;
		Event*					m_readEvent;
		Event*					m_writeEvent;
		ListeningSocket*		m_listener;
		t_ptr_ProtoConnection	m_ptr_protoConnection;				// <- the http connection
		t_ptr_ProtoModule		m_ptr_protoModule;					// <- the http module in our case

		// owned by the connection
		Nginx_MemoryPool&		m_memPool;
		Globals&				m_globals;
																				// set by the listening socket

};



#endif
