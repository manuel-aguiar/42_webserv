/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocketFactory.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 09:44:39 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:55:08 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../Concrete/ServerSocketFactory.hpp"

static UniquePtr<IServerSocket>    IPv4create(const struct addrinfo& info)
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
    return (UniquePtr<IServerSocket>(new ServerSocket <IPv4Address> (ipv4Addr, info.ai_socktype, info.ai_protocol)));
}

static UniquePtr<IServerSocket>    IPv6create(const struct addrinfo& info)
{
    (void)info;
    return (NULL);
}

UniquePtr<IServerSocket>    ServerSocketFactory::create(const struct addrinfo& info)
{
    switch (info.ai_family)
    {
        case (AF_INET):
            return (IPv4create(info));
        case (AF_INET6):
            return (IPv6create(info));
        default:
            return (NULL);
    }
    return (NULL);
}
