/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:37:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 17:02:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Socket.hpp"


Socket::Socket(int fd, char ip[INET6_ADDRSTRLEN], int port, int family) :
    _fd(fd), _port(port), _addrFamily(family)
{
    std::memcpy(_ip, ip, INET6_ADDRSTRLEN);
}

Socket::Socket() : _fd(-1), _port(-1), _addrFamily(-1) {}
Socket::~Socket() {}
Socket::Socket(const Socket& copy) : _fd(copy._fd), _port(copy._port),_addrFamily(copy._addrFamily)
 
 {
    std::memcpy(_ip, copy._ip, INET6_ADDRSTRLEN);
 }

Socket& Socket::operator=(const Socket& assign)
{
    if (this == &assign)
        return (*this);

    _fd = assign._fd;
    std::memcpy(_ip, assign._ip, INET6_ADDRSTRLEN);
    _port = assign._port;
    _addrFamily = assign._addrFamily;
    return (*this);
}

int             Socket::getFd() const { return (_fd); }
const char*     Socket::getIp() const { return (_ip); }
int             Socket::getPort() const { return (_port); }



static Socket    IPv4create(const struct addrinfo& info)
{

    char    ipstr[INET6_ADDRSTRLEN];
    void    *addr;
    int     port;
    int     sockFd;
    
    struct sockaddr_in* ipv4 = (struct sockaddr_in *)info.ai_addr;
    addr = &(ipv4->sin_addr);
    port = ntohs(ipv4->sin_port);
    if(!inet_ntop(info.ai_family, addr, ipstr, sizeof (ipstr)))
        throw ParameterException("Socket::createServerSocket", "inet_ntop", std::strerror(errno));
    sockFd = socket(info.ai_family, info.ai_socktype, info.ai_protocol);
    if (sockFd == -1)
        throw ParameterException("Socket::createServerSocket", "socket", std::strerror(errno));
    Socket  serverSocket(sockFd, ipstr, port, info.ai_family);
    if (bind(serverSocket.getFd(), info.ai_addr, info.ai_addrlen) == -1)
        throw ParameterException("Socket::createServerSocket", "bind", std::strerror(errno));
    if (listen(serverSocket.getFd(), 10) == -1)
        throw ParameterException("Socket::createServerSocket", "listen", std::strerror(errno));

    return (serverSocket);
}

static Socket    IPv6create(const struct addrinfo& info)
{
    return (Socket());
}

Socket    Socket::createServerSocket(const struct addrinfo& info)
{
    Socket serverSocket;
    
    switch (info.ai_family)
    {
        case (AF_INET):
            return(IPv4create(info));
            break;
    }

    

    return (Socket());
}