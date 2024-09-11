/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:23:50 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/11 09:45:48 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerSocket.hpp"

ServerSocket::ServerSocket(const ISocketAddress& addr, int type, int protocol) : 
    _fd(socket(addr.getAddrFamily(), type, protocol)), 
    _addr(addr.clone())
{
    if (_fd == -1)
        throw std::runtime_error("Failed to create socket");
}




ServerSocket::ServerSocket()
{
    
}