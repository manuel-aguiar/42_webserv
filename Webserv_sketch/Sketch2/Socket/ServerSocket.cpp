/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:53:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:55:35 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include "ClientSocket.hpp"

ServerSocket::ServerSocket(int domain, int type, int protocol) : Socket(domain, type, protocol) {}
ServerSocket::ServerSocket(const ServerSocket& other) : Socket(other) {}
ServerSocket& ServerSocket::operator=(const ServerSocket& other)
{
    if (this != &other)
    {
        Socket::operator=(other);
    }
    return *this;
}
ServerSocket::~ServerSocket() {}


