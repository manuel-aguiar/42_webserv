/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketAddressFactory.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:45:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/10 14:04:27 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETADDRESSFACTORY_HPP

# define SOCKETADDRESSFACTORY_HPP

# include "ISocketAddress.hpp"
# include "IPv4Address.hpp"
# include "IPv6Address.hpp"

class SocketAddressFactory
{
    public:
        static ISocketAddress* createSocketAddress(int domain);
};

#endif