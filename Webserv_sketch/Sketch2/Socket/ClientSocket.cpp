/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:53:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 17:22:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(const int listeningFd, const ISocketAddress& addr) : Socket(addr)
{
    _fd = accept(listeningFd, _addr->getSockAddr(), _addr->getAddrLen());
    if (_fd == -1)
        throw std::runtime_error(std::string("ClientSocket construction failed, accept(): ") + std::strerror(errno));
}

ClientSocket::ClientSocket(const ClientSocket& other) : Socket(other) {}


ClientSocket& ClientSocket::operator=(const ClientSocket& other)
{
    if (this != &other)
        Socket::operator=(other);
    return *this;
}

ClientSocket::~ClientSocket()
{
    
}