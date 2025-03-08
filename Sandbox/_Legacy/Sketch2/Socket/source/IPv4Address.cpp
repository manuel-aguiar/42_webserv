/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPv4Address.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:40:53 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/10/09 09:12:06 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Concrete/IPv4Address.hpp"

IPv4Address::IPv4Address(const struct sockaddr_in& addr) : m_addr(addr), _addrLen(sizeof(m_addr)) {}

IPv4Address::IPv4Address(const std::string& ip, uint16_t port)
{
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    if (inet_pton(m_addr.sin_family, ip.c_str(), &m_addr.sin_addr) != 1)
        throw std::runtime_error(std::string("IPv4Address construction failed, inet_pton(): ") + std::strerror(errno));
}

IPv4Address::IPv4Address(uint32_t ip, uint16_t port)
{
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    m_addr.sin_addr.s_addr = htonl(ip);
}

int IPv4Address::getAddrFamily() const
{
    return (m_addr.sin_family);
}

IPv4Address::IPv4Address(const IPv4Address& copy) : m_addr(copy.m_addr), _addrLen(sizeof(copy.m_addr)) {}

IPv4Address& IPv4Address::operator=(const IPv4Address& assign)
{
    if (this != &assign)
        m_addr = assign.m_addr;
    return *this;
}

IPv4Address& IPv4Address::operator=(const struct sockaddr_in& assign)
{
    if (&m_addr != &assign)
        m_addr = assign;
    return *this;
}

IPv4Address::~IPv4Address()
{
    
}

struct sockaddr*    IPv4Address::getSockAddr()
{
    return (reinterpret_cast<struct sockaddr*>(&m_addr));
}

socklen_t*          IPv4Address::getAddrLen()
{
    return (&_addrLen);
}

UniquePtr<ISocketAddress>     IPv4Address::clone() const
{
    return (UniquePtr<ISocketAddress>(new IPv4Address(*this)));
}


// private default constructor
IPv4Address::IPv4Address() : m_addr((struct sockaddr_in) {}), _addrLen(sizeof(m_addr)) {}