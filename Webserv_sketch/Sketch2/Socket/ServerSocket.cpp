/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:23:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 17:55:09 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerSocket.hpp"
# include "CommunicationSocket.hpp"

ServerSocket::ServerSocket(const ISocketAddress& addr, int type, int protocol) : 
    ASocket(socket(addr.getAddrFamily(), type, protocol), addr)
{
    int opt = 1;
    if (_fd == -1)
        throw ParameterException("SocketServer constructor failed", "socket", std::strerror(errno));
    if (_addr.get() == NULL)
        throw ParameterException("SocketServer constructor failed", "new", std::strerror(errno));
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        throw ParameterException("SocketServer constructor failed", "setsockopt", std::strerror(errno));
}

void    ServerSocket::bind()
{
    if (::bind(_fd, _addr->getSockAddr(), *(_addr->getAddrLen())) == -1)
        throw ParameterException("ServerSocket::bind", "bind", std::strerror(errno));
}

void    ServerSocket::listen()
{
    if (::listen(_fd, 10) == -1)
        throw ParameterException("ServerSocket::listen", "listen", std::strerror(errno));
}

UniquePtr<ACommunicationSocket> ServerSocket::accept()
{
    UniquePtr<ISocketAddress>               newAddr = _addr->clone();
    
    int newFd = ::accept(_fd, newAddr->getSockAddr(), newAddr->getAddrLen());
    if (newFd == -1)
        return (NULL);
    return (UniquePtr<ACommunicationSocket> (new CommunicationSocket(newFd, *newAddr)));
}

ServerSocket::~ServerSocket() {}
ServerSocket::ServerSocket() : ASocket() {}
ServerSocket::ServerSocket(ServerSocket& moveCopy) : ASocket(moveCopy) {}
ServerSocket& ServerSocket::operator=(ServerSocket& moveAssign)
{
    ASocket::operator=(moveAssign);
    return (*this);
}