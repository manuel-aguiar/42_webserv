/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommunicationSocket.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:39:57 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 12:45:57 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CommunicationSocket.hpp"

CommunicationSocket::CommunicationSocket() : ASocket() {}
CommunicationSocket::CommunicationSocket(int fd, const ISocketAddress& addr) : ASocket(fd, addr) {}
CommunicationSocket::~CommunicationSocket() {}

void    CommunicationSocket::send()
{
    write(_fd, "Hello, World!", 13);
}

void    CommunicationSocket::receive()
{
    
}

CommunicationSocket::CommunicationSocket(CommunicationSocket& copy) : ASocket(copy) {}

CommunicationSocket& CommunicationSocket::operator=(CommunicationSocket& assign)
{
    ASocket::operator=(assign);
    return (*this);
}

