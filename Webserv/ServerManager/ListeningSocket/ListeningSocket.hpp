/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/12/18 09:43:16 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP


// Project Headers
# include "../../GenericUtils/Webserver_Definitions.h"
# include "../../Globals/LogFile/LogFile.hpp"
# include "../../Event/Event.hpp"
# include "../EventManager/EventManager.hpp"

// C headers
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

//forward declarations
class Event;
class ServerWorker;
class Connection;
class Globals;

class ListeningSocket
{
	public:
		ListeningSocket(ServerWorker&		worker, 
						Nginx_MemoryPool&	memPool, 
						const t_addrinfo&	addrInfo, 
						const int			backlog, 
						Globals&			globals);
		~ListeningSocket();


		// methods
		int                         open();
		int                        	accept();

		void                        close();
		void                        closeConnection(Connection& connection);

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
		void						setProtoModule					(const t_ptr_ProtoModule& module);
		void						setInitProtocolConnection		(const t_func_initProtoConn& func);

		// accessors
		ServerWorker&				accessWorker();
		Event&						accessEvent();

	private:

		t_addrinfo					m_addrInfo;
		t_socket					m_sockfd;
		int							m_backlog;

		t_func_initProtoConn		m_initConnection;
		t_ptr_ProtoModule			m_protoModule;
		Event						m_event;
		ServerWorker&				m_worker;
		Globals&                    m_globals;

		ListeningSocket();
		ListeningSocket(const ListeningSocket& copy);
		ListeningSocket& operator=(const ListeningSocket& assign);


		int    mf_close_accepted_connection(Connection& connection);
		int    mf_bind();
		int    mf_listen();

};


#endif
