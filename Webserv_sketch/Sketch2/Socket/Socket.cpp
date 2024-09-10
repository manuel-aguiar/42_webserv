/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:52:59 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:34:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int domain, int type, int protocol) : FileDescriptor(), _addr(NULL)
{
    _fd = socket(domain, type, protocol);
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