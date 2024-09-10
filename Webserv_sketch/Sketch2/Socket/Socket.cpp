/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:52:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 14:03:26 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/*

    Socket in reality is only responsible for setting up the address family struct
    Socket opening is done on ServerSocket, calling the socket() function
    ServerSocket will create ClientSocket via accept() function
*/

Socket::Socket(int addressFamily) : FileDescriptor(), _addr(NULL)
{
    _addr = SocketAddressFactory::createSocketAddress(addressFamily);
    if (!_addr)
        throw std::runtime_error("Socket constructor failed, domain not supported");
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
        delete (_addr);
    _addr = NULL;
    
    if (assign._addr)
        _addr = assign._addr->clone();
    
    return (*this);
}

Socket::~Socket()
{
    if (_addr)
        delete (_addr);
}

Socket::Socket() : FileDescriptor() {}