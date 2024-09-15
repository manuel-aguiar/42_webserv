/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:51:17 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 10:00:39 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ASocket.hpp"

ASocket::ASocket(int fd, const ISocketAddress& addr) : 
    FileDescriptor(fd), 
    _addr(addr.clone().release()) {}

ASocket::~ASocket()
{
    if (_addr)
        delete _addr;
    _addr = NULL;
}

ASocket::ASocket() : FileDescriptor(-1), _addr(NULL) {}

ASocket::ASocket(const ASocket& hardCopy) : FileDescriptor(hardCopy), _addr(hardCopy._addr->clone().release()) {}

ASocket& ASocket::operator=(const ASocket& hardAssign)
{
    if (this == &hardAssign)
        return (*this);
    FileDescriptor::operator=(hardAssign);
    _addr = hardAssign._addr;
    return (*this);
}