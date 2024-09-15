/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:46:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 10:01:41 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(const ISocketAddress& addr, int type, int protocol) :
    ASocket(socket(addr.getAddrFamily(), type, protocol), addr)
{
    
}

ClientSocket::~ClientSocket()
{
    if (_fd != -1)
        close(_fd);
}


void ClientSocket::connect()
{
    if (::connect(_fd, _addr->getSockAddr(), *(_addr->getAddrLen())) == -1)
        throw ParameterException("ClientSocket::connect", "connect", std::strerror(errno));
}

void ClientSocket::disconnect()
{
    
}

void ClientSocket::send()
{
    // Implementation of send() method
}

void ClientSocket::receive()
{
    char buffer[1024];

        int bytesReceived = read(_fd, buffer, sizeof(buffer) - 1);
        if (bytesReceived == -1)
            throw ParameterException("ClientSocket::receive", "read", std::strerror(errno));
        buffer[bytesReceived] = '\0';
        std::cout << "Received: " << std::string(buffer, 0, bytesReceived) << std::endl;
}


void    ClientSocket::onClose() {}
void    ClientSocket::onRead() {}
void    ClientSocket::onWrite() {}


ClientSocket::ClientSocket() : ASocket() {}
ClientSocket::ClientSocket(const ClientSocket& moveCopy) : ASocket(moveCopy) {}
ClientSocket& ClientSocket::operator=(const ClientSocket& moveAssign)
{
    ASocket::operator=(moveAssign);
    return (*this);
}