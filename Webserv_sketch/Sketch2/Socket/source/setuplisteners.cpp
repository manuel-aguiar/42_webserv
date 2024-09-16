/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setuplisteners.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaria-d <mmaria-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 07:50:32 by mmaria-d          #+#    #+#             */
/*   Updated: 2024/09/16 10:01:00 by mmaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Concrete/ServerSocket.hpp"
#include "../Concrete/ClientSocket.hpp"
#include "../Concrete/SocketAddressFactory.hpp"
#include "../Concrete/ServerSocketFactory.hpp"
#include "../Concrete/IPv4Address.hpp"
#include "../Concrete/IPv6Address.hpp"
#include "../Concrete/CommunicationSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#include <vector>

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

int setupListeners()
{
    FileDescriptorManager fdManager;
    
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *cur;

    hints = (struct addrinfo){};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(NULL, "8080", &hints, &res);
	if (status != 0)
	{
		std::cerr << "getaddrinfo(): " + std::string(gai_strerror(status)) << std::endl;
		return (1);
	}
    
    for(cur = res; cur != NULL; cur = cur->ai_next)
	{
        try
        {
            UniquePtr<IServerHandle> server = ServerSocketFactory::create(*cur);
            server->bind();
            server->listen();
            server->setFdManager(&fdManager);
            fdManager.addFileDescriptor(server, true);
        }
        catch(const std::exception& e)
        {
            //alloc failed, non critical...?
            std::cerr << e.what() << '\n';
        }
    }   
    freeaddrinfo(res);
}


