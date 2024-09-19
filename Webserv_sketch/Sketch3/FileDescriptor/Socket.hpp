/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:37:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 16:57:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP    

# define SOCKET_HPP

# include <cstddef>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include "ParameterException.hpp"
# include <netdb.h>
# include <arpa/inet.h>
# include <cerrno>

class Socket
{
    public:
        Socket();
        Socket(int fd, char ip[INET6_ADDRSTRLEN], int port, int family);
        ~Socket();
        Socket(const Socket& copy);
        Socket& operator=(const Socket& assign);


        int             getFd() const;
        const char*     getIp() const;
        int             getPort() const;
        

        static Socket createServerSocket(const struct addrinfo& info);

    private:
    
        int                                 _fd;
        char                                _ip[INET6_ADDRSTRLEN];
        int                                 _port;
        int                                 _addrFamily;


};


#endif