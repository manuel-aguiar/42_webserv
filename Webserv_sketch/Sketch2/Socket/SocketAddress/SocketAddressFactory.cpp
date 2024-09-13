/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketAddressFactory.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:47:28 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/13 16:09:58 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "SocketAddressFactory.hpp"

UniquePtr<ISocketAddress> SocketAddressFactory::createSocketAddress(int domain)
{
    switch (domain)
    {
        case (AF_INET):
            return (UniquePtr<ISocketAddress>(new IPv4Address()));
        default:
            return (NULL);
    }
    return (NULL);
}