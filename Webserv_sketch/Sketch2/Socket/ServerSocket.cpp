/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:53:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 08:57:59 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"
#include "ClientSocket.hpp"

ServerSocket::ServerSocket() : Socket() {}
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


