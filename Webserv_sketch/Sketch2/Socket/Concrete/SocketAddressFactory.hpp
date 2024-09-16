/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketAddressFactory.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 11:45:43 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 07:52:53 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETADDRESSFACTORY_HPP

# define SOCKETADDRESSFACTORY_HPP


# include "../../../../LibftCpp/libftcpp.hpp"
# include "../Abstract/ISocketAddress.hpp"
# include "IPv4Address.hpp"
# include "IPv6Address.hpp"

class SocketAddressFactory
{
    public:
        static ISocketAddress*     createSocketAddress(int domain);
};

#endif