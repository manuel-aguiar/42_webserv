/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:51:17 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 14:23:23 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ASocket.hpp"

ASocket::ASocket(const int fd, const ISocketAddress& addr) : FileDescriptor(fd), _addr(addr.clone()) {}

ASocket::~ASocket()
{
    if (_addr != NULL)
        delete _addr;
}

// all private
ASocket::ASocket() : FileDescriptor(-1), _addr(NULL) {}
ASocket::ASocket(const ASocket& copy) : FileDescriptor(copy), _addr(copy._addr->clone()) {}
ASocket& ASocket::operator=(const ASocket& assign)
{
    if (this == &assign)
        return (*this);
    FileDescriptor::operator=(assign);
    if (_addr != NULL)
        delete _addr;
    _addr = assign._addr->clone();
    return (*this);
}