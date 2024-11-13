/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/15 20:33:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "../Webserver_Definitions.h"
# include "../Logs/Logs.h"
# include "../Event/Event.hpp"

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>


class Server;
class Connection;
class ConnectionPool;
class EventManager;
class Globals;

class ListeningSocket
{
    public:
        ListeningSocket(ConnectionPool& connPool, EventManager& m_eventManager, Globals* globals);
        ~ListeningSocket();

        int                         open();
        int                         bind();
        int                         listen();
        void                        accept();
        void                        close();

        void                        closeConnection(Connection* connection);

        Globals*                    m_globals;

        
        int                         m_socktype;    
        t_socket                    m_sockfd;
        int                         m_proto;
        t_sockaddr*                 m_addr;
        t_socklen                   m_addrlen;
        int                         m_backlog;
        

        
        Event                       m_myEvent;    
        ConnectionPool&             m_connectionPool;
        EventManager&               m_eventManager;
    

    private:
        ListeningSocket();
        ListeningSocket(const ListeningSocket& copy);
        ListeningSocket& operator=(const ListeningSocket& assign);


        void    _close_accepted_connection(Connection* connection);

};


#endif