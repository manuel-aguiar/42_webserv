/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/03 12:54:57 by mmaria-d         ###   ########.fr       */
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
        ListeningSocket(ConnectionPool& connPool, EventManager& _eventManager, Globals* globals);
        ~ListeningSocket();

        int                         open();
        int                         bind();
        int                         listen();
        void                        accept();
        void                        close();

        Globals*                    _globals;
        int                         _socktype;    
        t_socket                    _sockfd;
        int                         _proto;
        t_sockaddr*                 _addr;
        t_socklen                   _addrlen;
        int                         _backlog;
        
        Event                       _myEvent;    
        ConnectionPool&             _connectionPool;
        EventManager&               _eventManager;
    

    private:
        ListeningSocket();
        ListeningSocket(const ListeningSocket& copy);
        ListeningSocket& operator=(const ListeningSocket& assign);


        void    _close_accepted_connection(Connection* connection);

};


#endif