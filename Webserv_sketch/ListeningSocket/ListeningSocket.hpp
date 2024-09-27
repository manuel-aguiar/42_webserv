/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListeningSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 14:50:33 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/27 14:52:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENINGSOCKET_HPP

# define LISTENINGSOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

typedef int t_fd;

class ListeningSocket
{
    public:

        ListeningSocket();
        ~ListeningSocket();

        void    bind();
        void    listen();
        void    accept();
        void    close();
    
    private:
        t_fd                _sockfd;
        struct sockaddr*    _addr;
        socklen_t           _addrlen;
        int                 _backlog;
};


#endif