/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocketFactory.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 08:06:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 09:04:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKETFACTORY_HPP

# define SERVERSOCKETFACTORY_HPP

#include "../Concrete/ServerSocket.hpp"
#include "../../FileDescriptorManager/FileDescriptorManager.hpp"
#include "SocketAddressFactory.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class ServerSocketFactory
{
    public:
        template <typename SockAddr>
        static UniquePtr<ServerSocket<SockAddr> >    create(const struct addrinfo& info)
        {
            switch (info.ai_family)
            {
                case (AF_INET):
                    return (ServerSocketFactory::create<IPv4Address>(info));
                case (AF_INET6):
                    return (ServerSocketFactory::create<IPv6Address>(info));
                default:
                    return (NULL);
            }
            return (NULL);
        }

        template <typename IPv4Address>
        static UniquePtr<ServerSocket<IPv4Address> >    create<IPv4Address>(const struct addrinfo& info)
        {
            char    ipstr[INET6_ADDRSTRLEN];
            void    *addr;
            int     port;
            
            struct sockaddr_in* ipv4 = (struct sockaddr_in *)info.ai_addr;
            addr = &(ipv4->sin_addr);
            port = ntohs(ipv4->sin_port);
            if(!inet_ntop(info.ai_family, addr, ipstr, sizeof (ipstr)))
                return (NULL);
            IPv4Address ipv4Addr(ipstr, port);
            return (make_UniquePtr<ServerSocket <IPv4Address> >(new ServerSocket <IPv4Address> (ipv4Addr, info.ai_socktype, info.ai_protocol)));
        }

        template <typename IPv6Address>
        static UniquePtr<ServerSocket<IPv6Address> >    create<IPv4Address>(const struct addrinfo& info)
        {
            return (NULL);
        }
};

#endif