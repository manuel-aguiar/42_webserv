/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPv4Address.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:40:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:09:28 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IPv4Address.hpp"

IPv4Address::IPv4Address(const sockaddr_in& addr) : _addr(addr) {}

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

IPv4Address::IPv4Address(const IPv4Address& other) : _addr(other._addr) {}

IPv4Address& IPv4Address::operator=(const IPv4Address& other)
{
    if (this != &other)
        _addr = other._addr;
    return *this;
}

IPv4Address::~IPv4Address()
{
    
}

struct sockaddr*    IPv4Address::getSockAddr()
{
    return (reinterpret_cast<struct sockaddr*>(&_addr));
}

socklen_t           IPv4Address::getSize() const
{
    return sizeof(_addr);
}



// private default constructor
IPv4Address::IPv4Address() {}