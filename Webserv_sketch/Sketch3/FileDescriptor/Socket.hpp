/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:37:56 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 12:46:34 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP    

# define SOCKET_HPP

class Socket
{
    public:
        Socket();
        ~Socket();

        typedef enum SockAddrType
        {
            IPV4,
            IPV6,
            SOCKADDR_COUNT
        }   SockAddrType;

        int             getFd() const;
        const char*     getIp() const;
        int             getPort() const;
        
    private:
    
        int             _fd;
        const char      _ip;
        int             _port;
        SockAddrType    _addrType;

        ServerSocket(const ServerSocket& copy);
        ServerSocket& operator=(const ServerSocket& assign);
};


#endif