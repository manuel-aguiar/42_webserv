/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setuplisteners.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 07:50:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 07:58:59 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Concrete/ServerSocket.hpp"
#include "../Concrete/ClientSocket.hpp"
#include "../Concrete/SocketAddressFactory.hpp"
#include "../Concrete/IPv4Address.hpp"
#include "../Concrete/IPv6Address.hpp"
#include "../Concrete/CommunicationSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*

    struct addrinfo {
        int              ai_flags;
        int              ai_family;
        int              ai_socktype;
        int              ai_protocol;
        socklen_t        ai_addrlen;
        struct sockaddr *ai_addr;
        char            *ai_canonname;
        struct addrinfo *ai_next;
    };
*/

void setupListeners()
{
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *cur;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
}