/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 12:37:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/19 12:47:24 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Socket.hpp"

Socket::Socket() : _fd(-1), _ip(NULL), _port(-1), _addrType(SOCKADDR_COUNT) {}
Socket::~Socket() {}
Socket::Socket(const Socket& copy) : _fd(copy._fd), _ip(copy._ip), _port(copy._port),_addrType(copy._addrType) {}
Socket& Socket::operator=(const Socket& assign)
{
    if (this == &assign)
        return (*this);

    _fd = assign._fd;
    _ip = assign._ip;
    _port = assign._port;
    _addrType = assign._addrType;
    return (*this);
}

int             Socket::getFd() const { return (_fd); }
const char*     Socket::getIp() const { return (_ip); }
int             Socket::getPort() const { return (_port); }