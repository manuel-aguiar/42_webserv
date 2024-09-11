/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:46:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:49:02 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(const ISocketAddress& addr, int type, int protocol) :
    ASocket(socket(addr.getAddrFamily(), type, protocol), addr)
{
    // Constructor implementation
}

ClientSocket::~ClientSocket()
{
    // Destructor implementation
    close();
}

void ClientSocket::close()
{
    if (_fd >= 0)
        ::close(_fd);
    _fd = -1;
    // Implementation of close() method
}

void ClientSocket::connect()
{
    // Implementation of connect() method
    if (::connect(_fd, _addr->getSockAddr(), *(_addr->getAddrLen())) == -1)
        throw ParameterException("ClientSocket::connect", "connect", std::strerror(errno));
}

void ClientSocket::disconnect()
{
    close();
    // Implementation of disconnect() method
}

void ClientSocket::send()
{
    // Implementation of send() method
}

void ClientSocket::receive()
{
    // Implementation of receive() method
}