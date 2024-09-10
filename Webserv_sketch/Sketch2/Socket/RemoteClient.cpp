/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RemoteClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:53:20 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 18:07:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RemoteClient.hpp"

RemoteClient::RemoteClient(int acceptfd, const Socket& socket) : _socket(socket)
{
    _socket.setFd(acceptfd);
}

RemoteClient::RemoteClient(const RemoteClient& copy) : _socket(copy._socket) {}


RemoteClient& RemoteClient::operator=(const RemoteClient& assign)
{
    if (this != &assign)
        _socket = assign._socket;
    return *this;
}

RemoteClient::~RemoteClient()
{
    
}