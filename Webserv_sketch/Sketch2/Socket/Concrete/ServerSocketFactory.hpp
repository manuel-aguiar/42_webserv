/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocketFactory.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 08:06:35 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 10:17:18 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERSOCKETFACTORY_HPP

# define SERVERSOCKETFACTORY_HPP

#include "../Concrete/ServerSocket.hpp"
#include "../../FileDescriptorManager/FileDescriptorManager.hpp"
#include "SocketAddressFactory.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class ServerSocketFactory
{
    public:
        static UniquePtr<IServerSocket>    create(const struct addrinfo& info);

};

#endif