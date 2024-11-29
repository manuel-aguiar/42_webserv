/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:55:54 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/29 10:18:47 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../../GenericUtils/Webserver_Definitions.h"

class Globals;
class ListeningSocket;
class Event;

class Connection
{
	public:
		Connection(Globals* globals = NULL);
		~Connection();

		void    				init();

		void    				reset();
		void    				read();
		void    				write();
		void    				close();

		t_ptr_ProtoConnection	getProtoConnection() const;
		t_ptr_ProtoModule		accessProtoModule() const;

		void					setProtoConnection(t_ptr_ProtoConnection connection);
		void					setProtoModule(t_ptr_ProtoModule module);

		void					setListener(ListeningSocket* listener);
		void					setMemPool(Nginx_MemoryPool* memPool);

		void					setReadEvent(Event* event);
		void					setWriteEvent(Event* event);

		t_socket				getSocket() const;
		t_sockaddr*				getAddr() const;
		t_socklen				getAddrlen() const;

		void					setSocket(t_socket sockfd);
		void					setAddr(t_sockaddr* addr);
		void					setAddrlen(t_socklen addrlen);

		Globals* 				getGlobals() const;

	private:
		t_socket				m_sockfd;
		t_sockaddr*				m_addr;
		t_socklen				m_addrlen;
		Event*					m_readEvent;
		Event*					m_writeEvent;
		ListeningSocket*		m_listener;
		Nginx_MemoryPool*		m_memPool;
		Globals*				m_globals;
		t_ptr_ProtoConnection	m_ptr_protoConnection;				// <- the http connection
		t_ptr_ProtoModule		m_ptr_protoModule;					// <- the http module in our case
																				// set by the listening socket

		Connection(const Connection& other);
		Connection& operator=(const Connection& other);

};



#endif
