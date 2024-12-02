/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/02 11:02:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Globals/LogFile/LogFile.hpp"
# include "../EventManager/EventManager.hpp"

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

class Event;
class ServerWorker;
class Connection;
class Globals;

class ListeningSocket
{
	public:
		ListeningSocket(ServerWorker& worker, Globals* globals);
		~ListeningSocket();

		// methods
		int                         open();
		int                         bind();
		int                         listen();
		void                        accept();
		void                        close();

		void                        closeConnection(Connection* connection);

		// events
		static void                 EventAccept(Event& event);


		// getters
		const ServerWorker&			getWorker()		const;
		t_socket					getSocket()		const;
		int							getSockType()	const;
		const t_sockaddr*			getAddr()		const;
		t_socklen					getAddrlen()	const;
		t_port						getPort()		const;
		int							getBacklog()	const;
		
		// setters
		void						setSocket(const t_socket sockfd);
		void						setSockType(const int socktype);
		void						setAddr(const t_sockaddr* sockaddr);
		void						setAddrlen(const t_socklen addrlen);
		void						setPort(const t_port port);
		void						setBacklog(const int backlog);

		// accessors
		ServerWorker&				accessWorker();



	private:


		int							m_socktype;
		t_socket					m_sockfd;
		int							m_proto;
		t_port						m_port;
		t_sockaddr*					m_addr;
		t_socklen					m_addrlen;
		int							m_backlog;

		typedef void				(*AcceptInitProtocol)(Connection *);
		AcceptInitProtocol			m_initConnection;

		t_ptr_ProtoModule			m_protoModule;

		Event						m_myEvent;
		ServerWorker&				m_worker;

		Globals*                    m_globals;

		ListeningSocket();
		ListeningSocket(const ListeningSocket& copy);
		ListeningSocket& operator=(const ListeningSocket& assign);


		void    mf_close_accepted_connection(Connection* connection);

};


#endif
