/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/30 10:16:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include "../Webserver_Definitions.h"
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>


class ListeningSocket
{
    public:
        ListeningSocket();
        ~ListeningSocket();

        int     open();
        int     bind();
        int     listen();
        void    accept();
        void    close();

        int                 _socktype;    
        t_socket            _sockfd;
        int                 _proto;
        t_sockaddr*         _addr;
        t_socklen           _addrlen;
        int                 _backlog;
};


#endif