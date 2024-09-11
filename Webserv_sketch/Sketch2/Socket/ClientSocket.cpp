/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:46:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 15:16:51 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(const ISocketAddress& addr, int type, int protocol) :
    ASocket(socket(addr.getAddrFamily(), type, protocol), addr)
{
    
}

ClientSocket::~ClientSocket()
{
    close();
}

void ClientSocket::close()
{
    FileDescriptor::close();
}

void ClientSocket::connect()
{
    if (::connect(_fd, _addr->getSockAddr(), *(_addr->getAddrLen())) == -1)
        throw ParameterException("ClientSocket::connect", "connect", std::strerror(errno));
}

void ClientSocket::disconnect()
{
    close();
}

void ClientSocket::send()
{
    // Implementation of send() method
}

void ClientSocket::receive()
{
    // Implementation of receive() method
}