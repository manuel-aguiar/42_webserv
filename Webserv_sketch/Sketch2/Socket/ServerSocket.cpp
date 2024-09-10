/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:53:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 14:07:14 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include "ClientSocket.hpp"

ServerSocket::ServerSocket(int domain, int type, int protocol) : Socket()
{
    _fd = socket(domain, type, protocol);
    if (_fd == -1)
        throw std::runtime_error(std::string("Socket constructor failed, socket(): ") + std::strerror(errno));
}

ServerSocket::ServerSocket(const ServerSocket& other) : Socket(other) {}
ServerSocket& ServerSocket::operator=(const ServerSocket& other)
{
    if (this != &other)
        Socket::operator=(other);
    return (*this);
}
ServerSocket::~ServerSocket() {}


