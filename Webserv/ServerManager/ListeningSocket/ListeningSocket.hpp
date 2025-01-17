/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rphuyal <rphuyal@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2025/01/18 14:49:20 by rphuyal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Globals/LogFile/LogFile.hpp"
# include "../../Event/Event.hpp"
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
		ListeningSocket(ServerWorker& worker, const t_sockaddr& listenAddr, Globals* globals);
		~ListeningSocket();

		// typedefs
		typedef void 				(*t_func_initProtoConn)(Connection*);

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
		const ServerWorker&			getWorker()						const;
		t_socket					getSocket()						const;
		int							getSockType()					const;
		int							getProtocol()					const;
		const t_sockaddr*			getAddr()						const;
		t_socklen					getAddrlen()					const;
		t_port						getPort()						const;
		int							getBacklog()					const;
		const Event&				getEvent()						const;

		// setters
		void						setSocket						(const t_socket sockfd);
		void						setSockType						(const int socktype);
		void						setProtocol						(const int proto);
		void						setAddr							(const t_sockaddr* sockaddr);
		void						setAddrlen						(const t_socklen addrlen);
		void						setPort							(const t_port port);
		void						setBacklog						(const int backlog);
		void						setProtoModule					(const t_ptr_ProtoModule module);
		void						setInitProtocolConnection		(const t_func_initProtoConn func);

		// accessors
		ServerWorker&				accessWorker();
		Event&						accessEvent();
		t_sockaddr*					accessAddr();


	private:

		int							m_socktype;
		t_socket					m_sockfd;
		int							m_proto;
		t_port						m_port;
		t_sockaddr*					m_addr;
		t_socklen					m_addrlen;
		int							m_backlog;

		t_func_initProtoConn		m_initConnection;

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
