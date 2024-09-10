/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocalClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 08:53:11 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 17:39:22 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocalClientSocket.hpp"
#include "RemoteServer.hpp"



void    LocalClientSocket::requestConnection(const RemoteServer& server) 
{
    if (connect(server.getFd(), reinterpret_cast<struct sockaddr*>(&server.getAddress()), *(server.getAddress().getAddrLen())) == -1)
        throw std::runtime_error(std::string("LocalClientSocket requestConnection() failed, connect(): ") + std::strerror(errno));
}


LocalClientSocket::~LocalClientSocket()
{

}

//private
LocalClientSocket::LocalClientSocket()
{

}

