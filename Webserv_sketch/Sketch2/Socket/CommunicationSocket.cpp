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

CommunicationSocket::CommunicationSocket(int fd, const ISocketAddress& addr) : ASocket(fd, addr)
{

}

CommunicationSocket::~CommunicationSocket()
{
    
}

void    CommunicationSocket::close()
{
    
}

void    CommunicationSocket::send()
{
    
}

void    CommunicationSocket::receive()
{
    
}

CommunicationSocket::CommunicationSocket(const CommunicationSocket& copy) : ASocket(copy)
{

}


CommunicationSocket& CommunicationSocket::operator=(const CommunicationSocket& assign)
{
    if (this == &assign)
        return (*this);
    return (*this);
}