/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:46:51 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 13:51:53 by mmaria-d         ###   ########.fr       */
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
}

void ClientSocket::close()
{
    // Implementation of close() method
}

void ClientSocket::connect()
{
    // Implementation of connect() method
}

void ClientSocket::disconnect()
{
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