/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:52:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 18:02:11 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/*

    Socket in reality is only responsible for setting up the address family struct
    Socket opening is done on ServerSocket, calling the socket() function
    ServerSocket will create ClientSocket via accept() function

*/
Socket::Socket() : FileDescriptor(), _addr(NULL)
{
    /*
        Does nothing, let the next derived call socket(), or for instance a ClientSocket
        that doesn't create the socket itself
    */
}

ISocketAddress&             Socket::getAddress() const
{
    return (*_addr);
}

void                        Socket::setAddress(const ISocketAddress& address)
{
    if (_addr)
        delete (_addr);
    _addr = address.clone();
}

Socket::Socket(int domain) : FileDescriptor(), _addr(NULL)
{
    _prepareSockAddr(domain);
}

Socket::Socket(const ISocketAddress& addr) : FileDescriptor(), _addr(NULL)
{
    _addr = addr.clone();
}

void    Socket::_prepareSockAddr(int domain)
{
    if (_addr)
        delete (_addr);
    _addr = SocketAddressFactory::createSocketAddress(domain);
}

Socket::Socket(int domain, int type, int protocol) : FileDescriptor(), _addr(NULL)
{
    _prepareSockAddr(domain);
    _fd = socket(domain, type, protocol);
    if (_fd == -1)
        throw std::runtime_error(std::string("Socket constructor failed, socket(): ") + std::strerror(errno));
}

Socket::Socket(const ISocketAddress& addr, int type, int protocol) : FileDescriptor(), _addr(NULL)
{
    _addr = addr.clone();
    _fd = socket(_addr->getAddrFamily(), type, protocol);
    if (_fd == -1)
        throw std::runtime_error(std::string("Socket constructor failed, socket(): ") + std::strerror(errno));
}

Socket::Socket(const Socket& copy) : FileDescriptor(copy), _addr(NULL)
{
    if (copy._addr)
        _addr = copy._addr->clone();
}

Socket& Socket::operator=(const Socket& assign)
{
    if (this == &assign)
        return (*this);

    FileDescriptor::operator=(assign);
    if (_addr)
    {
        delete (_addr);
        _addr = NULL;
    }
    
    if (assign._addr)
        _addr = assign._addr->clone();
    
    return (*this);
}

Socket::~Socket()
{
    if (_addr)
        delete (_addr);
    if (_fd != -1)
        close(_fd);
}

