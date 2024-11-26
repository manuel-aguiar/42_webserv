/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/11/26 09:03:32 by mmaria-d         ###   ########.fr       */
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
class ConnectionManager;
class EventManager;
class Globals;

class ListeningSocket
{
    public:
        ListeningSocket(ConnectionManager& connPool, EventManager& m_eventManager, Globals* globals);
        ~ListeningSocket();

        int                         open();
        int                         bind();
        int                         listen();
        void                        accept();
        void                        close();

        void                        closeConnection(Connection* connection);

        static void                 EventAccept(Event& event);

        Globals*                    m_globals;


        int                         m_socktype;
        t_socket                    m_sockfd;
        int                         m_proto;
        t_sockaddr*                 m_addr;
        t_socklen                   m_addrlen;
        int                         m_backlog;



        Event                       m_myEvent;
        ConnectionManager&          m_connectionPool;
        EventManager&               m_eventManager;


    private:
        ListeningSocket();
        ListeningSocket(const ListeningSocket& copy);
        ListeningSocket& operator=(const ListeningSocket& assign);


        void    mf_close_accepted_connection(Connection* connection);

};


#endif
