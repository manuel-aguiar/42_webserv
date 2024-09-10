/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:53:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 09:54:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(const ServerSocket& server) : Socket()
{
    (void)server;
}
ClientSocket::ClientSocket(const ClientSocket& other) : Socket(other) {}


ClientSocket& ClientSocket::operator=(const ClientSocket& other)
{
    if (this != &other)
        Socket::operator=(other);
    return *this;
}

ClientSocket::~ClientSocket() {}