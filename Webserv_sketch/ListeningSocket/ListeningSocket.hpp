/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/01 14:40:47 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "../Webserver_Definitions.h"
# include "../Logs/Logs.h"

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>


class Server;
class Connection;
class ConnectionPool;

class ListeningSocket
{
    public:
        ListeningSocket(ConnectionPool& connPool, ILog* logFile);
        ~ListeningSocket();

        int                         open();
        int                         bind();
        int                         listen();
        void                        accept();
        void                        close();

        ILog*                       _logFile;
        int                         _socktype;    
        t_socket                    _sockfd;
        int                         _proto;
        t_sockaddr*                 _addr;
        t_socklen                   _addrlen;
        int                         _backlog;
        
        Connection*                 _myConnection;    
        ConnectionPool&             _connectionPool;
};


#endif