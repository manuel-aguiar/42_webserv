/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketAddressFactory.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:47:28 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 11:55:40 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../Concrete/SocketAddressFactory.hpp"

ISocketAddress* SocketAddressFactory::createSocketAddress(int domain)
{
    switch (domain)
    {
        case (AF_INET):
            return (new IPv4Address());
        default:
            return (NULL);
    }
    return (NULL);
}