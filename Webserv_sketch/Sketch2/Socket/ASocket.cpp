/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASocket.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 11:51:17 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 17:44:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ASocket.hpp"

ASocket::ASocket(int fd, const ISocketAddress& addr) : 
    FileDescriptor(fd), 
    _addr(UniquePtr<ISocketAddress>(addr.clone())) {}

ASocket::~ASocket() {}

// all protected
ASocket::ASocket() : FileDescriptor(-1), _addr(NULL) {}

ASocket::ASocket(ASocket& copy) : FileDescriptor(copy), _addr(copy._addr) {}

ASocket& ASocket::operator=(ASocket& assign)
{
    if (this != &assign)
    {
        FileDescriptor::operator=(assign);
        _addr = assign._addr;
    }
    return (*this);
}

//copy not implemented, only move