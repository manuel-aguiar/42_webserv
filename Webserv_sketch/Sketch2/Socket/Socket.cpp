/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:52:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:59:19 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
/*

    This is WRONG
    ClientSocket cannot be derived from Socket because the creation is different
    It is the ServerSocket that calls socket(), not client.
    Client is born from the accept() call in ServerSocket
    
*/


Socket::Socket(int domain, int type, int protocol) : FileDescriptor(), _addr(NULL)
{
    _addr = SocketAddressFactory::createSocketAddress(domain);
    if (!_addr)
        throw std::runtime_error("Socket constructor failed, domain not supported");
    _fd = socket(domain, type, protocol);
    if (_fd == -1)
        throw std::runtime_error(std::string("Socket constructor failed, socket(): ") + std::strerror(errno));
}

Socket::Socket(const Socket& copy) : FileDescriptor(copy)
{
    _addr = copy._addr->clone();
}

Socket& Socket::operator=(const Socket& assign)
{
    if (this == &assign)
        return (*this);

    FileDescriptor::operator=(assign);
    if (_addr)
        delete (_addr);
    _addr = assign._addr->clone();
    
    return (*this);
}

Socket::~Socket()
{
    if (_addr)
        delete (_addr);
}

Socket::Socket() : FileDescriptor()
{
    
}