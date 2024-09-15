/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:23:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/15 16:44:46 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerSocket.hpp"
# include "CommunicationSocket.hpp"

/*

UniquePtr<ACommunicationSocket>    ServerSocket::accept()
{
    UniquePtr<ISocketAddress>               newAddr = UniquePtr<ISocketAddress> (_addr->clone());
    
    int newFd = ::accept(_fd, newAddr->getSockAddr(), newAddr->getAddrLen());
    if (newFd == -1)
        return (NULL);
    return (UniquePtr<ACommunicationSocket>(new CommunicationSocket(newFd, *newAddr)));
}

ServerSocket::~ServerSocket()
{
    if (_fd != -1)
        close(_fd);
}
*/
    