/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocalServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:53:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 17:39:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocalServerSocket.hpp"
#include "RemoteClient.hpp"

LocalServerSocket::LocalServerSocket(const ISocketAddress& addr, int type, int protocol, int maxConnections) : Socket(addr, type, protocol)
{
    
}

LocalServerSocket::LocalServerSocket(const LocalServerSocket& other) : Socket(other) {}


LocalServerSocket& LocalServerSocket::operator=(const LocalServerSocket& other)
{
    if (this != &other)
        Socket::operator=(other);
    return (*this);
}

void    LocalServerSocket::bindListen(int maxConnections)
{
    if (bind(_fd, _addr->getSockAddr(), *(_addr->getAddrLen())) == -1)
        throw std::runtime_error(std::string("LocalServerSocket bindListen() failed, bind(): ") + std::strerror(errno));
    if (listen(_fd, maxConnections) == -1)
        throw std::runtime_error(std::string("LocalServerSocket bindListen() failed, listen(): ") + std::strerror(errno));
}

RemoteClient*   LocalServerSocket::acceptConnection()
{
    Socket* newSocket = new Socket(getAddress());

    int newFd = accept(
            _fd, 
            reinterpret_cast<struct sockaddr*>(newSocket->getAddress().getSockAddr()), 
            newSocket->getAddress().getAddrLen());
    return (new RemoteClient(_fd, *_addr));
}

LocalServerSocket::~LocalServerSocket()
{

}

//private
LocalServerSocket::LocalServerSocket() : Socket()
{

}

