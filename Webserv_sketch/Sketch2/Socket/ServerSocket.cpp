/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:23:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:06:59 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerSocket.hpp"
# include "CommunicationSocket.hpp"

ServerSocket::ServerSocket(const ISocketAddress& addr, int type, int protocol) : ASocket(socket(addr.getAddrFamily(), type, protocol), addr)
{
    if (_fd != -1)
        throw ParameterException("SocketServer constructor failed", "socket", std::strerror(errno));
    if (_addr == NULL)
        throw ParameterException("SocketServer constructor failed", "new", std::strerror(errno));
}


ServerSocket& ServerSocket::operator=(const ServerSocket& assign)
{
    if (this == &assign)
        return (*this);
    return (*this);
}

bool    ServerSocket::operator<(const ServerSocket& other) const
{
    return (_fd < other._fd);
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

CommunicationSocket* ServerSocket::accept()
{
    CommunicationSocket*    newSocket = NULL;
    ISocketAddress*         newAddr = _addr->clone();
    
    int newFd = ::accept(_fd, newAddr->getSockAddr(), newAddr->getAddrLen());
    if (newFd == -1)
    {
        delete (newAddr);
        return (newSocket);
    }
    newSocket = new (std::nothrow) CommunicationSocket(newFd, *newAddr);
    delete (newAddr);
    return (newSocket);
}

void    ServerSocket::close()
{
    if (_fd != -1)
        ::close(_fd);
}

ServerSocket::~ServerSocket()
{
    if (_addr != NULL)
        delete _addr;
    close();
}

//private
ServerSocket::ServerSocket() {}
ServerSocket::ServerSocket(const ServerSocket& copy) {}