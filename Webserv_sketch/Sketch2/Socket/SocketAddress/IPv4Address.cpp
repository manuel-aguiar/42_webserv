/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPv4Address.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:40:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 16:08:05 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IPv4Address.hpp"

IPv4Address::IPv4Address(const struct sockaddr_in& addr) : _addr(addr), _addrLen(sizeof(_addr)) {}

IPv4Address::IPv4Address(const std::string& ip, uint16_t port)
{
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    if (inet_pton(_addr.sin_family, ip.c_str(), &_addr.sin_addr) != 1)
        throw std::runtime_error(std::string("IPv4Address construction failed, inet_pton(): ") + std::strerror(errno));
}

IPv4Address::IPv4Address(uint32_t ip, uint16_t port)
{
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = htonl(ip);
}

int IPv4Address::getAddrFamily() const
{
    return (_addr.sin_family);
}

IPv4Address::IPv4Address(const IPv4Address& copy) : _addr(copy._addr), _addrLen(sizeof(copy._addr)) {}

IPv4Address& IPv4Address::operator=(const IPv4Address& assign)
{
    if (this != &assign)
        _addr = assign._addr;
    return *this;
}

IPv4Address& IPv4Address::operator=(const struct sockaddr_in& assign)
{
    if (&_addr != &assign)
        _addr = assign;
    return *this;
}

IPv4Address::~IPv4Address()
{
    
}

struct sockaddr*    IPv4Address::getSockAddr()
{
    return (reinterpret_cast<struct sockaddr*>(&_addr));
}

socklen_t*          IPv4Address::getAddrLen()
{
    return (&_addrLen);
}

UniquePtr<ISocketAddress>     IPv4Address::clone() const
{
    return (UniquePtr<ISocketAddress> (new IPv4Address(*this)));
}


// private default constructor
IPv4Address::IPv4Address() : _addr((struct sockaddr_in) {}) {}