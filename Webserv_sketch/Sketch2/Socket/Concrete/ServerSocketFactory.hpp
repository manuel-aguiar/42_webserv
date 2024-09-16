/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocketFactory.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 08:06:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 15:17:50 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKETFACTORY_HPP

# define SERVERSOCKETFACTORY_HPP

#include "../Concrete/ServerSocket.hpp"
#include "../../FileDescriptorManager/FileDescriptorManager.hpp"
#include "IPv4Address.hpp"
#include "IPv6Address.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class ServerSocketFactory
{
    public:
        static UniquePtr<IServerSocket>    create(const struct addrinfo& info);

};

#endif