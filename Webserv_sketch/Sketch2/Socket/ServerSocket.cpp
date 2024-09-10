/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:53:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 17:22:04 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include "ClientSocket.hpp"

ServerSocket::ServerSocket(const ISocketAddress& addr, int type, int protocol, int maxConnections) : Socket(addr, type, protocol)
{

}


ServerSocket::ServerSocket(const ServerSocket& other) : Socket(other) {}


ServerSocket& ServerSocket::operator=(const ServerSocket& other)
{
    if (this != &other)
        Socket::operator=(other);
    return (*this);
}

void    ServerSocket::bindListen(int maxConnections)
{
    if (bind(_fd, _addr->getSockAddr(), *(_addr->getAddrLen())) == -1)
        throw std::runtime_error(std::string("ServerSocket bindListen() failed, bind(): ") + std::strerror(errno));
    if (listen(_fd, maxConnections) == -1)
        throw std::runtime_error(std::string("ServerSocket bindListen() failed, listen(): ") + std::strerror(errno));
}

ClientSocket*   ServerSocket::accept()
{
    return (new ClientSocket(_fd, *_addr));
}

ServerSocket::~ServerSocket()
{

}



//private
ServerSocket::ServerSocket() : Socket()
{

}

