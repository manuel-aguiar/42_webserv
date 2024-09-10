/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketAddressFactory.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:47:28 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 11:49:47 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "SocketAddressFactory.hpp"

ISocketAddress* SocketAddressFactory::createSocketAddress(int domain)
{
    switch (domain)
    {
        case (AF_INET):
            return (new IPv4Address());
            break ;
        default:
            return (NULL);
    }
    return (NULL);
}